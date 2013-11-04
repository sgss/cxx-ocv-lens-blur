//
//  sgss/gradient_filter.cc
//
//  MIT License
//
//  Copyright (C) 2013 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#include "sgss/gradient_filter.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>

#include "sgss/color.h"
#include "sgss/quadtree.h"

namespace sgss {

GradientFilter::GradientFilter(const cv::Mat& kernel, const cv::Size& size,
                               double lower_range, double upper_range)
    : gradient_(),
      range_(lower_range, upper_range),
      filters_() {
  BuildFilters(kernel, size);
}

GradientFilter::GradientFilter(const GradientFilter& other)
    : gradient_(other.gradient_),
      range_(other.range_),
      filters_(other.filters_) {}

GradientFilter::~GradientFilter() {}

GradientFilter& GradientFilter::operator=(const GradientFilter& other) {
  if (&other != this) {
    gradient_ = other.gradient_;
    range_ = other.range_;
    filters_ = other.filters_;
  }
  return *this;
}

void GradientFilter::operator()(const cv::Mat& source, cv::Mat *destination) {
  assert(!source.empty());
  assert(source.channels() == 3);
  const cv::Mat *source_ptr = &source;
  cv::Mat3f inter_source;
  if (source.depth() != cv::DataDepth<float>::value) {
    source.convertTo(inter_source, cv::DataType<float>::type);
    source_ptr = &inter_source;
  }
  cv::Mat3f inter_destination(source.size());
  if (gradient_.empty()) {
    filters_.back()->apply(*source_ptr, inter_destination);
  } else {
    const double size = range_.second - range_.first;
    assert(size != 0.0);
    const double interval = size / (filters_.size() + 1);
    Quadtree tree(source.size());
    tree.Insert(gradient_, interval);
    ApplyInRect(tree, interval, *source_ptr, gradient_, &inter_destination);
  }
  inter_destination.convertTo(*destination, source.type());
}

void GradientFilter::BuildFilters(const cv::Mat& kernel, const cv::Size& size) {
  assert(!kernel.empty());
  assert(kernel.channels() == 1);
  const cv::Mat *kernel_ptr = &kernel;
  cv::Mat1f inter_kernel;
  if (kernel.depth() != cv::DataDepth<float>::value) {
    kernel.convertTo(inter_kernel, cv::DataType<float>::type);
    kernel_ptr = &inter_kernel;
  }
  cv::Size ksize;
  if (size.width <= 0 || size.height <= 0) {
    ksize = kernel.size();
  } else {
    ksize = size;
  }
  assert(ksize.width % 2 == 1 && ksize.height % 2 == 1);
  assert(filters_.empty());
  while (ksize.width > 0 && ksize.height > 0) {
    cv::Mat1f subkernel;
    cv::resize(*kernel_ptr, subkernel, ksize, 0.0, 0.0, cv::INTER_AREA);
    cv::normalize(subkernel, subkernel, 1.0, 0.0, cv::NORM_L1);
    filters_.push_back(cv::createLinearFilter(
        cv::DataType<cv::Vec3f>::type,
        cv::DataType<cv::Vec3f>::type,
        subkernel));
    ksize.width -= 2;
    ksize.height -= 2;
  }
  std::reverse(filters_.begin(), filters_.end());
}

void GradientFilter::ApplyInRect(const Quadtree& tree, double interval,
                                 const cv::Mat3f& source,
                                 const cv::Mat1f& gradient,
                                 cv::Mat3f *destination) const {
  if (!tree.empty()) {
    const auto end = tree.end();
    for (auto itr = tree.begin(); itr != end; ++itr) {
      ApplyInRect(**itr, interval, source, gradient, destination);
    }
  } else {
    const cv::Rect& rect = tree.rect();
    const cv::Mat3f source_roi(source, rect);
    const cv::Mat1f gradient_roi(gradient, rect);
    cv::Mat3f destination_roi(*destination, rect);

    // Determine filter indices for the lower and upper value boundaries.
    // This could be negative when the source image doesn't need to be
    // filtered.
    const index_type lower_index = std::min<index_type>(
        std::floor(tree.min_value() / interval),
        filters_.size()) - 1;
    const index_type upper_index = std::min<index_type>(
        std::ceil(tree.max_value() / interval),
        filters_.size()) - 1;
    assert(lower_index <= upper_index);

    // The maximum value of the gradient image doesn't reach the lower value
    // boundary. Simply copy the source to the destination.
    if (upper_index < 0) {
      source_roi.copyTo(destination_roi);

    // The minimum value of the gradient image exceeds the upper value boundary.
    // Filter with the largest kernel since no need to composite.
    } else if (lower_index == upper_index) {
      Apply(source_roi, lower_index, &destination_roi);

    // Values of the gradient span more than one boundary.
    } else {
      if (lower_index < 0) {
        source_roi.copyTo(destination_roi);
      } else {
        Apply(source_roi, lower_index, &destination_roi);
      }
      // Nodes that span more than two boundaries are exceptional cases, where
      // the gradient might be too complex to subdivide. Complex gradients
      // however could be approximated by just compositing lower and upper
      // filter results, ignoring intermediating filters.
      if (upper_index - lower_index > 2) {
        const double lower_value = (lower_index + 1) * interval;
        const double upper_value = (upper_index + 1) * interval;
        ApplyComposite(source_roi, upper_index, destination_roi, gradient_roi,
                       lower_value, upper_value, &destination_roi);
      } else {
        for (index_type index = lower_index + 1;
             index <= upper_index; ++index) {
          const double lower_value = index * interval;
          const double upper_value = (index + 1) * interval;
          ApplyComposite(source_roi, index, destination_roi, gradient_roi,
                         lower_value, upper_value, &destination_roi);
        }
      }
    }
  }
}

void GradientFilter::ApplyComposite(const cv::Mat3f& source,
                                    index_type filter_index,
                                    const cv::Mat3f& underlay,
                                    const cv::Mat1f& gradient,
                                    double lower_value, double upper_value,
                                    cv::Mat3f *destination) const {
  // Map values of the gradient between the lower and upper value boundaries
  // within 0.0 - 1.0.
  cv::Mat1f alpha_channel;
  cv::subtract(gradient, lower_value, alpha_channel);
  cv::divide(alpha_channel, upper_value - lower_value, alpha_channel);
  cv::max(alpha_channel, 0.f, alpha_channel);
  cv::min(alpha_channel, 1.f, alpha_channel);
  cv::Mat3f alpha;
  cv::merge(std::vector<cv::Mat1f>(source.channels(), alpha_channel), alpha);

  // Filter overlay image with kernels for the given index.
  cv::Mat3f overlay(source.size(), source.type());
  Apply(source, filter_index, &overlay);

  // Perform standard alpha composition.
  cv::multiply(overlay, alpha, overlay);
  cv::multiply(underlay, cv::Scalar(1.0, 1.0, 1.0) - alpha, *destination);
  cv::add(overlay, *destination, *destination);
}

void GradientFilter::Apply(const cv::Mat3f& source, index_type filter_index,
                           cv::Mat3f *destination) const {
  cv::Ptr<cv::FilterEngine> filter(filters_.at(filter_index));
  filter->apply(source, *destination);
}

}  // namespace sgss
