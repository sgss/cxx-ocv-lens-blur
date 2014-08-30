//
//  sgss/color.h
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
#ifndef SGSS_COLOR_H_
#define SGSS_COLOR_H_

#ifdef __cplusplus

#include <opencv2/opencv.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>

extern void *enabler;

namespace sgss {
namespace color {
namespace constants {

template <
  typename T,
  typename std::enable_if<
    std::is_integral<T>::value
  >::type *& = enabler
>
constexpr T max() {
  return std::numeric_limits<T>::max();
}

template <
  typename T,
  typename std::enable_if<
    std::is_floating_point<T>::value
  >::type *& = enabler
>
constexpr T max() {
  return 1.0;
}

inline double max(int depth) {
  switch (depth) {
    case cv::DataDepth<std::uint8_t>::value:
      return max<std::uint8_t>();
    case cv::DataDepth<std::int8_t>::value:
      return max<std::int8_t>();
    case cv::DataDepth<std::uint16_t>::value:
      return max<std::uint16_t>();
    case cv::DataDepth<std::int16_t>::value:
      return max<std::int16_t>();
    case cv::DataDepth<std::int32_t>::value:
      return max<std::int32_t>();
    case cv::DataDepth<float>::value:
      return max<float>();
    case cv::DataDepth<double>::value:
      return max<double>();
    default:
      assert(false);
  }
}

template <
  typename T,
  typename std::enable_if<
    std::is_integral<T>::value
  >::type *& = enabler
>
constexpr T min() {
  return std::numeric_limits<T>::min();
}

template <
  typename T,
  typename std::enable_if<
    std::is_floating_point<T>::value
  >::type *& = enabler
>
constexpr T min() {
  return 0.0;
}

inline double min(int depth) {
  switch (depth) {
    case cv::DataDepth<std::uint8_t>::value:
      return min<std::uint8_t>();
    case cv::DataDepth<std::int8_t>::value:
      return min<std::int8_t>();
    case cv::DataDepth<std::uint16_t>::value:
      return min<std::uint16_t>();
    case cv::DataDepth<std::int16_t>::value:
      return min<std::int16_t>();
    case cv::DataDepth<std::int32_t>::value:
      return min<std::int32_t>();
    case cv::DataDepth<float>::value:
      return min<float>();
    case cv::DataDepth<double>::value:
      return min<double>();
    default:
      assert(false);
  }
}

template <
  typename T,
  typename std::enable_if<
    std::is_floating_point<T>::value
  >::type *& = enabler
>
constexpr T size() {
  return 1.0;
}

template <
  typename T,
  typename std::enable_if<
    std::is_integral<T>::value
  >::type *& = enabler
>
constexpr unsigned int size() {
  return static_cast<unsigned int>(max<T>()) - min<T>();
}

inline double size(int depth) {
  switch (depth) {
    case cv::DataDepth<std::uint8_t>::value:
      return size<std::uint8_t>();
    case cv::DataDepth<std::int8_t>::value:
      return size<std::int8_t>();
    case cv::DataDepth<std::uint16_t>::value:
      return size<std::uint16_t>();
    case cv::DataDepth<std::int16_t>::value:
      return size<std::int16_t>();
    case cv::DataDepth<std::int32_t>::value:
      return size<std::int32_t>();
    case cv::DataDepth<float>::value:
      return size<float>();
    case cv::DataDepth<double>::value:
      return size<double>();
    default:
      assert(false);
  }
}

}  // namespace constants
}  // namespace color
}  // namespace sgss

#endif  // __cplusplus

#endif  // SGSS_COLOR_H_
