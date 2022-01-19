#!/bin/bash
# 1. first find all source files
# 2. remove copywrite
find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' \
  -not -path "*/cpi/*" -not -path "*/cmake-build-debug/*" \
  -not -name "*CLI11.hpp" -not -name "*matplotlibcpp.h" \
  -exec sed -i '1,/\*\//{ /\/\*/,/\*\//d }' {} \;

# 3. Create copytext file
COPYHEADER="/*******************************************************
 * This file is part of PISCES.
 * Author: Leo
 *******************************************************/
"
echo "$COPYHEADER" > /tmp/out_copy


# 4. now append the new header to the files!
find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' \
  -not -path "*/cpi/*" -not -path "*/cmake-build-debug/*" \
  -not -name "*CLI11.hpp" -not -name "*matplotlibcpp.h" \
  -print0 |
  while IFS= read -r -d '' file; do
    echo $file;
    cat /tmp/out_copy $file > /tmp/out && mv /tmp/out $file
  done
