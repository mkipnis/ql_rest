#!/bin/bash

git clone -b master https://github.com/mkipnis/ql_rest ql_rest

cd ql_rest

cmake .. -DQLA_ROOT_DIR=/usr/local -DCMAKE_INSTALL_PREFIX=/usr/local

mkdir -p build
cd build
cmake --build . --target install --config Debug -v
