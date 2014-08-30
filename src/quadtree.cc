//
//  sgss/quadtree.cc
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

#include "sgss/quadtree.h"

#include <opencv2/opencv.hpp>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <utility>

namespace sgss {

void Quadtree::Insert(const cv::Mat& matrix, double interval,
                      const cv::Size& size_limit, std::size_t max_span) {
  assert(matrix.channels() == 1);
  assert(interval > 0.0);
  assert(max_span > 0);
  cv::minMaxIdx(cv::Mat(matrix, rect_), &min_value_, &max_value_);

  // Subdivide this node when values in the region of interest of the matrix
  // span more than the number of value boundaries defined by max_span.
  if (rect_.width > size_limit.width &&
      rect_.height > size_limit.height &&
      max_value_ > (std::floor(min_value_ / interval) + max_span) * interval) {
    Subdivide();
    for (auto& node : nodes_) {
      node->Insert(matrix, interval, size_limit, max_span);
    }
  }
}

void Quadtree::Subdivide() {
  if (empty_) {
    using Value = cv::Rect::value_type;
    const Value x1 = rect_.x;
    const Value y1 = rect_.y;
    const Value w1 = cv::saturate_cast<Value>(rect_.width * 0.5);
    const Value h1 = cv::saturate_cast<Value>(rect_.height * 0.5);
    const Value x2 = x1 + w1;
    const Value y2 = y1 + h1;
    const Value w2 = rect_.width - w1;
    const Value h2 = rect_.height - h1;
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x1, y1, w1, h1)));
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x2, y1, w2, h1)));
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x1, y2, w1, h2)));
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x2, y2, w2, h2)));
    empty_ = false;
  }
}

}  // namespace sgss
