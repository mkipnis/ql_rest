#!/bin/bash

set -x 

mkdir -p build
cd build

#cmake $CMAKE_FLAGS .. -DQLA_ROOT_DIR=/tmp -DCMAKE_INSTALL_PREFIX=/tmp/ql_rest_install_dir
cmake $CMAKE_FLAGS .. -DCMAKE_INSTALL_PREFIX=/tmp/ql_rest_install_dir

cmake --build . --target install --config Debug -v
