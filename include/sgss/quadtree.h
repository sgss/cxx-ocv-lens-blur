//
//  sgss/quadtree.h
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

#ifndef SGSS_QUADTREE_H_
#define SGSS_QUADTREE_H_

#ifdef __cplusplus

#include <cstddef>
#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>

class Quadtree {
 public:
  typedef unsigned int level_type;
  typedef std::unique_ptr<Quadtree> pointer_type;
  typedef typename std::vector<pointer_type>::const_iterator const_iterator;

  // Constructors
  Quadtree();
  Quadtree(const cv::Size& size);
  Quadtree(const cv::Rect& rect);
  Quadtree(const Quadtree& other);
  
  // Assignment
  Quadtree& operator=(const Quadtree& other);

  // Inserts all elements in the given single-channel matrix.
  bool Insert(const cv::Mat& matrix, double interval,
              const cv::Size& size_limit = cv::Size(8, 8),
              std::size_t max_span = 2);

  // Getting information about this node.
  level_type level() const;
  const cv::Rect& rect() const;
  bool empty() const;

  // Holds value boundaries.
  double min_value() const;
  double max_value() const;

  // Iterator to quadrant nodes.
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

 private:
  Quadtree(level_type level, const cv::Rect& rect);
  void Subdivide();

  level_type level_;
  cv::Rect rect_;
  bool empty_;
  double min_value_;
  double max_value_;
  std::vector<pointer_type> nodes_;
};

#pragma mark -

inline Quadtree::level_type Quadtree::level() const {
  return level_;
}

inline const cv::Rect& Quadtree::rect() const {
  return rect_;
}

inline bool Quadtree::empty() const {
  return empty_;
}

inline double Quadtree::min_value() const {
  return min_value_;
}

inline double Quadtree::max_value() const {
  return max_value_;
}

inline Quadtree::const_iterator Quadtree::begin() const {
  return nodes_.begin();
}

inline Quadtree::const_iterator Quadtree::end() const {
  return nodes_.end();
}

inline Quadtree::const_iterator Quadtree::cbegin() const {
  return nodes_.cbegin();
}

inline Quadtree::const_iterator Quadtree::cend() const {
  return nodes_.cend();
}

#endif  // __cplusplus

#endif  // SGSS_QUADTREE_H_
