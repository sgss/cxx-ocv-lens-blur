//
//  sgss/lens_blur_filter.cc
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

#include "sgss/lens_blur_filter.h"

#include "sgss/color.h"

namespace sgss {

LensBlurFilter::LensBlurFilter(const cv::Mat& kernel, const cv::Size& size)
    : GradientFilter(kernel, size),
      brightness_(1.f) {}

LensBlurFilter::LensBlurFilter(const LensBlurFilter& other)
    : GradientFilter(other),
      brightness_(other.brightness_) {}

LensBlurFilter::~LensBlurFilter() {}

LensBlurFilter& LensBlurFilter::operator=(const LensBlurFilter& other) {
  GradientFilter::operator=(other);
  if (&other != this) {
    brightness_ = other.brightness_;
  }
  return *this;
}

void LensBlurFilter::operator()(const cv::Mat& source, cv::Mat *destination) {
  assert(!source.empty());
  assert(source.channels() == 3);
  assert(brightness_ != 0.f);

  // Make intermediate exponential image.
  cv::Mat3f source_exp;
  source.convertTo(source_exp, cv::DataDepth<float>::value);
  source_exp *= brightness_ / color::constants::max(source.depth());
  cv::exp(source_exp, source_exp);

  cv::Mat3f destination_exp(source.size());
  GradientFilter::operator()(source_exp, &destination_exp);

  // Log the exponential image back to linear.
  cv::log(destination_exp, destination_exp);
  destination_exp *= color::constants::max(source.depth()) / brightness_;
  destination_exp.convertTo(*destination, source.depth());
}

}  // namespace sgss
