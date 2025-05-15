#!/bin/bash

set -x 

mkdir -p build
cd build

cmake $CMAKE_FLAGS .. -DCMAKE_INSTALL_PREFIX=$HOME
cmake --build . --target install --config Debug -v
