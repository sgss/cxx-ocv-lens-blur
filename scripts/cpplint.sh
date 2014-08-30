#!/bin/sh

readonly SRCROOT=$(cd "$(dirname "$0")/../"; pwd)

for FILE in $(find "$SRCROOT/src" -type f -name "*.cc" -o -name "*.c"); do
  python "$SRCROOT/cpplint/cpplint.py" --root src "$FILE"
done
for FILE in $(find "$SRCROOT/include" -type f -name "*.h"); do
  python "$SRCROOT/cpplint/cpplint.py" --root include "$FILE"
done
for FILE in $(find "$SRCROOT/tests" -type f -name "*.cc"); do
  python "$SRCROOT/cpplint/cpplint.py" --root tests "$FILE"
done
