//
//  sgss/lens_blur_filter.h
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

#ifndef SGSS_LENS_BLUR_FILTER_H_
#define SGSS_LENS_BLUR_FILTER_H_

#ifdef __cplusplus

#include "sgss/gradient_filter.h"

#include <algorithm>
#include <cmath>

namespace sgss {

class LensBlurFilter : public GradientFilter {
 public:
  // Constructors
  LensBlurFilter(const cv::Mat& kernel, const cv::Size& size = cv::Size());
  LensBlurFilter(const LensBlurFilter& other);
  virtual ~LensBlurFilter();

  // Assignment
  LensBlurFilter& operator=(const LensBlurFilter& other);

  // Overrides GradientFilter::operator().
  virtual void operator()(const cv::Mat& source, cv::Mat *destination);

  // Brightness of specular highlight. 0.f < value and 1.f for no effect.
  float brightness() const;
  void set_brightness(float value);

 private:
  float brightness_;
};

#pragma mark -

inline float LensBlurFilter::brightness() const {
  return brightness_;
}

inline void LensBlurFilter::set_brightness(float value) {
  brightness_ = std::max(value, std::nexttoward(0.f, 1.f));
}

}  // namespace sgss

#endif  // __cplusplus

#endif  // SGSS_LENS_BLUR_FILTER_H_
