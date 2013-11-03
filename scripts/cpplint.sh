#!/bin/bash

PROJECT_DIR=$(cd "$(dirname "$0")/../"; pwd)

for FILE in `find ${PROJECT_DIR}/src -type f -name "*.cc"` ; do
  python ${PROJECT_DIR}/cpplint/cpplint.py \
      --root src \
      --filter=-build/include_order,-readability/braces \
      ${FILE}
done

for FILE in `find ${PROJECT_DIR}/include -type f -name "*.h"` ; do
  python ${PROJECT_DIR}/cpplint/cpplint.py \
      --root include \
      --filter=-build/include_order,-readability/braces \
      ${FILE}
done
