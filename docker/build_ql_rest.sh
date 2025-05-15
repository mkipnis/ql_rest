#!/bin/bash

git clone -b cmake https://github.com/mkipnis/ql_rest ql_rest

cd ql_rest
mkdir -p build
cd build

cmake .. -DQLA_ROOT_DIR=/usr/local -DCMAKE_INSTALL_PREFIX=/usr/local

cmake --build . --target install --config Debug -v

# Python whl
mkdir /usr/local/dist
cd /ql_rest/qlrest/python_package
python3 -m build 
cp dist/ql_rest-0.0.1-py3-none-any.whl /usr/local/dist/
