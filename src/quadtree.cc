//
//  sgss/quadtree.cc
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

#include "sgss/quadtree.h"

#include <cmath>
#include <cstdint>
#include <utility>

namespace sgss {

Quadtree::Quadtree()
    : level_(0),
      rect_(),
      empty_(true),
      min_value_(0.0),
      max_value_(0.0),
      nodes_() {}

Quadtree::Quadtree(const cv::Size& size)
    : level_(0),
      rect_(cv::Point(), size),
      empty_(true),
      min_value_(0.0),
      max_value_(0.0),
      nodes_() {}

Quadtree::Quadtree(const cv::Rect& rect)
    : level_(0),
      rect_(rect),
      empty_(true),
      min_value_(0.0),
      max_value_(0.0),
      nodes_() {}

Quadtree::Quadtree(level_type level, const cv::Rect& rect)
    : level_(level),
      rect_(rect),
      empty_(true),
      min_value_(0.0),
      max_value_(0.0),
      nodes_() {}

Quadtree::Quadtree(const Quadtree& other)
    : level_(other.level_),
      rect_(other.rect_),
      empty_(other.empty_),
      min_value_(other.min_value_),
      max_value_(other.max_value_),
      nodes_() {
  const auto end = other.nodes_.cend();
  for (auto itr = other.nodes_.cbegin(); itr != end; ++itr) {
    nodes_.emplace_back(new Quadtree(**itr));
  }
}

Quadtree& Quadtree::operator=(const Quadtree& other) {
  if (&other != this) {
    level_ = other.level_;
    rect_ = other.rect_;
    empty_ = other.empty_;
    min_value_ = other.min_value_;
    max_value_ = other.max_value_;
    nodes_.clear();
    const auto end = other.nodes_.cend();
    for (auto itr = other.nodes_.cbegin(); itr != end; ++itr) {
      nodes_.emplace_back(new Quadtree(**itr));
    }
  }
  return *this;
}

void Quadtree::Insert(const cv::Mat& matrix, double interval,
                      const cv::Size& size_limit, std::size_t max_span) {
  assert(matrix.channels() == 1);
  assert(interval > 0.0);
  assert(max_span > 0);
  cv::minMaxIdx(cv::Mat(matrix, rect_), &min_value_, &max_value_);

  // Subdivide this node when values in the region of interest of the matrix
  // span more than the number of value boundaries defined by max_span.
  if (rect_.width > size_limit.width && rect_.height > size_limit.height &&
      max_value_ > (std::floor(min_value_ / interval) + max_span) * interval) {
    Subdivide();
    const auto end = nodes_.end();
    for (auto itr = nodes_.begin(); itr != end; ++itr) {
      (*itr)->Insert(matrix, interval);
    }
  }
}

void Quadtree::Subdivide() {
  if (empty_) {
    using value_type = cv::Rect::value_type;
    const value_type x1 = rect_.x;
    const value_type y1 = rect_.y;
    const value_type w1 = cv::saturate_cast<value_type>(rect_.width * 0.5f);
    const value_type h1 = cv::saturate_cast<value_type>(rect_.height * 0.5f);
    const value_type x2 = x1 + w1;
    const value_type y2 = y1 + h1;
    const value_type w2 = rect_.width - w1;
    const value_type h2 = rect_.height - h1;
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x1, y1, w1, h1)));
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x2, y1, w2, h1)));
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x1, y2, w1, h2)));
    nodes_.emplace_back(new Quadtree(level_ + 1, cv::Rect(x2, y2, w2, h2)));
    empty_ = false;
  }
}

}  // namespace sgss
