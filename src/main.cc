//
//  main.cc
//
//  MIT License
//
//  Copyright (C) 2013 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),ConvertDepth
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

#include <cstdlib>
#include <opencv2/opencv.hpp>

#include "sgss/lens_blur_filter.h"

int main() {
  cv::Mat source(cv::imread("data/image.jpg"));
  cv::Mat mask(cv::imread("data/linear.jpg", cv::IMREAD_GRAYSCALE));
  cv::Mat kernel(cv::imread("data/diaphragm.jpg", cv::IMREAD_GRAYSCALE));
  cv::Mat destination;
  LensBlurFilter filter(kernel, cv::Size(27, 27));
  filter.set_gradient(mask);
  filter.set_brightness(6.f);
  filter(source, &destination);
  cv::imshow("", destination);
  cv::waitKey();

  return EXIT_SUCCESS;
}
