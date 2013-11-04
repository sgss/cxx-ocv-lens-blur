# OpenCV Lens Blur

![](http://gyazo.com/797e3b939b0c9fc6457084409b2523d0.png)

---

- [sgss::Quadtree](include/sgss/quadtree.h)
- [sgss::Filter](include/sgss/filter.h)
- [sgss::GradientFilter](include/sgss/gradient_filter.h)
- [sgss::LensBlurFilter](include/sgss/lens_blur_filter.h)

## Usage

```cpp
#include <cstdlib>
#include <opencv2/opencv.hpp>

#include "sgss/lens_blur_filter.h"

int main() {
  const cv::Mat source(cv::imread("data/image.jpg"));
  const cv::Mat mask(cv::imread("data/linear.jpg", cv::IMREAD_GRAYSCALE));
  const cv::Mat kernel(cv::imread("data/diaphragm.jpg", cv::IMREAD_GRAYSCALE));
  cv::Mat destination;
  sgss::LensBlurFilter filter(kernel, cv::Size(27, 27));
  filter.set_gradient(mask);
  filter.set_brightness(6.f);
  filter(source, &destination);
  cv::imshow("", destination);
  cv::waitKey();

  return EXIT_SUCCESS;
}
```

## Prerequisites

You need a OpenCV library compiled with STL that supports C++11 features.
Configuring it with cmake by setting CMAKE_CXX_FLAGS as "-stdlib=libc++".
See [scripts/install_opencv.sh](scripts/install_opencv.sh) to install OpenCV,
or [scripts/build_opencv.sh](scripts/build_opencv.sh) for building for bundling
it in your application.

## Style Guide

This project tries to conform to [Google's C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml) except:

- Use of C++11 lambda expressions
- Use of exceptions in exceptional cases
- Use of streams when strongly needed for convenience
- Irregular order of include directives in rational cases

## License


MIT License

Copyright (c) 2013 Shota Matsuda

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

### data/image.jpg

[Cars on the Street](http://www.flickr.com/photos/camknows/9320258091/)
(c) by [camknows](http://www.flickr.com/photos/camknows/)

Cars on the Street is licensed under a Creative Commons 
Attribution-NonCommercial-ShareAlike 2.0 Unported License.
You should have received a copy of the license along with this
work. If not, see <http://creativecommons.org/licenses/by-sa/3.0/>.
