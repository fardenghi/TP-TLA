#! /bin/bash

set -euxo pipefail

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

rm -f -r "$BASE_PATH/build"
rm -f "$BASE_PATH/src/main/c/frontend/lexical-analysis/FlexScanner.c"
rm -f "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.c"
rm -f "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.h"

cmake -S . -B build
cd build
make
cd ..

echo "All done."
