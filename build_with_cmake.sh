#!/bin/bash

set -x 

mkdir -p build
cd build

cmake $CMAKE_FLAGS .. -DCMAKE_INSTALL_PREFIX=$HOME -DQLA_ROOT_DIR=$HOME
cmake --build . --target install --config Debug -v
