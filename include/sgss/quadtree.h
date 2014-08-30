//
//  sgss/quadtree.h
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
#ifndef SGSS_QUADTREE_H_
#define SGSS_QUADTREE_H_

#ifdef __cplusplus

#include <opencv2/opencv.hpp>

#include <cstddef>
#include <memory>
#include <vector>

namespace sgss {

class Quadtree {
 public:
  using Level = unsigned int;
  using Storage = std::vector<std::unique_ptr<Quadtree>>;

  // Constructors
  Quadtree();
  explicit Quadtree(const cv::Size& size);
  explicit Quadtree(const cv::Rect& rect);
  Quadtree(const Quadtree& other);

  // Assignment
  Quadtree& operator=(const Quadtree& other);

  // Inserts all elements in the given single-channel matrix
  void Insert(const cv::Mat& matrix,
              double interval,
              const cv::Size& size_limit = cv::Size(8, 8),
              std::size_t max_span = 2);

  // Getting information about this node
  Level level() const { return level_; }
  const cv::Rect& rect() const { return rect_; }
  bool empty() const { return empty_; }

  // Holds value boundaries
  double min_value() const { return min_value_; }
  double max_value() const { return max_value_; }

  // Iterator to quadrant nodes
  Storage::const_iterator begin() const { return nodes_.begin(); }
  Storage::const_iterator end() const { return nodes_.end(); }
  Storage::const_iterator cbegin() const { return nodes_.cbegin(); }
  Storage::const_iterator cend() const { return nodes_.cend(); }

 private:
  // Constructors
  Quadtree(Level level, const cv::Rect& rect);

  // Subdivides this quadtree
  void Subdivide();

  // Data members
  Level level_;
  cv::Rect rect_;
  bool empty_;
  double min_value_;
  double max_value_;
  Storage nodes_;
};

#pragma mark - Inline Implementations

inline Quadtree::Quadtree()
    : level_(),
      empty_(true),
      min_value_(),
      max_value_() {}

inline Quadtree::Quadtree(const cv::Size& size)
    : level_(),
      rect_(cv::Point(), size),
      empty_(true),
      min_value_(),
      max_value_() {}

inline Quadtree::Quadtree(const cv::Rect& rect)
    : level_(),
      rect_(rect),
      empty_(true),
      min_value_(),
      max_value_() {}

inline Quadtree::Quadtree(Level level, const cv::Rect& rect)
    : level_(level),
      rect_(rect),
      empty_(true),
      min_value_(),
      max_value_() {}

inline Quadtree::Quadtree(const Quadtree& other)
    : level_(other.level_),
      rect_(other.rect_),
      empty_(other.empty_),
      min_value_(other.min_value_),
      max_value_(other.max_value_),
      nodes_() {
  for (const auto& node : other.nodes_) {
    nodes_.emplace_back(new Quadtree(*node));
  }
}

inline Quadtree& Quadtree::operator=(const Quadtree& other) {
  if (&other != this) {
    level_ = other.level_;
    rect_ = other.rect_;
    empty_ = other.empty_;
    min_value_ = other.min_value_;
    max_value_ = other.max_value_;
    nodes_.clear();
    for (const auto& node : other.nodes_) {
      nodes_.emplace_back(new Quadtree(*node));
    }
  }
  return *this;
}

}  // namespace sgss

#endif  // __cplusplus

#endif  // SGSS_QUADTREE_H_
