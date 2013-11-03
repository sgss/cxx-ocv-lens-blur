//
//  sgss/gradient_filter.h
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

#ifndef SGSS_GRADIENT_FILTER_H_
#define SGSS_GRADIENT_FILTER_H_

#ifdef __cplusplus

#include <cassert>
#include <utility>
#include <vector>
#include <opencv2/opencv.hpp>

#include "sgss/filter.h"

class Quadtree;

class GradientFilter : public Filter {
 public:
  typedef std::make_signed<std::size_t>::type index_type;

  // Constructors
  GradientFilter(const cv::Mat& kernel, const cv::Size& size = cv::Size(),
                 double lower_range = 0.0, double upper_range = 255.0);
  GradientFilter(const GradientFilter& other);
  virtual ~GradientFilter();

  // Assignment
  GradientFilter& operator=(const GradientFilter& other);

  // Overrides Filter::operator().
  virtual void operator()(const cv::Mat& source, cv::Mat *destination);

  // Matrix of gradient.
  const cv::Mat& gradient() const;
  void set_gradient(const cv::Mat& value);

  // Value range for all the matrices to apply with.
  const std::pair<double, double>& range() const;
  void set_range(double min, double max);

 private:
  // Builds filter engines for all the value boundaries.
  void BuildFilters(const cv::Mat& kernel, const cv::Size& size);

  // Recursively applies filters on partial region of the source defined by
  // rectangle of the given quadrant nodes.
  void ApplyInRect(const Quadtree& tree, double interval,
                   const cv::Mat3f& source, const cv::Mat1f& gradient,
                   cv::Mat3f *destination) const;

  // Applies filter at the given index, and alpha-composite it with the
  // underlay by the gradient as alpha.
  void ApplyComposite(const cv::Mat3f& source, index_type filter_index,
                      const cv::Mat3f& underlay, const cv::Mat1f& gradient,
                      double lower_value, double upper_value,
                      cv::Mat3f *destination) const;

  // Applies filter at the given index.
  void Apply(const cv::Mat3f& source, index_type filter_index,
             cv::Mat3f *destination) const;

  cv::Mat1f gradient_;
  std::pair<double, double> range_;
  std::vector<cv::Ptr<cv::FilterEngine>> filters_;
};

#pragma mark -

inline const cv::Mat& GradientFilter::gradient() const {
  return gradient_;
}

inline void GradientFilter::set_gradient(const cv::Mat& value) {
  assert(value.channels() == 1);
  if (value.depth() != cv::DataDepth<float>::value) {
    value.convertTo(gradient_, cv::DataType<float>::type);
  } else {
    gradient_ = value;
  }
}

inline const std::pair<double, double>& GradientFilter::range() const {
  return range_;
}

inline void GradientFilter::set_range(double min, double max) {
  range_ = std::pair<double, double>(min, max);
}

#endif  // __cplusplus

#endif  // SGSS_GRADIENT_FILTER_H_
