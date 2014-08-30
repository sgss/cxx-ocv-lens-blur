//
//  sgss/lens_blur_filter.h
//
//  MIT License
//
//  Copyright (C) 2013-2014 Shota Matsuda
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

#pragma once
#ifndef SGSS_LENS_BLUR_FILTER_H_
#define SGSS_LENS_BLUR_FILTER_H_

#ifdef __cplusplus

#include <opencv2/opencv.hpp>

#include "sgss/gradient_filter.h"

namespace sgss {

class LensBlurFilter : public GradientFilter {
 public:
  // Constructors
  LensBlurFilter(const cv::Mat& kernel, const cv::Size& size = cv::Size());
  LensBlurFilter(const LensBlurFilter& other);

  // Assignment
  LensBlurFilter& operator=(const LensBlurFilter& other);

  // Performs filtering
  virtual void operator()(const cv::Mat& source, cv::Mat *destination);

  // Brightness of specular highlight. Negative or zero for no effect.
  float brightness() const { return brightness_; }
  void set_brightness(float value) { brightness_ = value; }

 private:
  // Data members
  float brightness_;
};

#pragma mark - Inline Implementations

inline LensBlurFilter::LensBlurFilter(const cv::Mat& kernel,
                                      const cv::Size& size)
    : GradientFilter(kernel, size),
      brightness_(1.0) {}

inline LensBlurFilter::LensBlurFilter(const LensBlurFilter& other)
    : GradientFilter(other),
      brightness_(other.brightness_) {}

inline LensBlurFilter& LensBlurFilter::operator=(const LensBlurFilter& other) {
  GradientFilter::operator=(other);
  if (&other != this) {
    brightness_ = other.brightness_;
  }
  return *this;
}

}  // namespace sgss

#endif  // __cplusplus

#endif  // SGSS_LENS_BLUR_FILTER_H_
