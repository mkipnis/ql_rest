#!/bin/bash

git clone https://github.com/mkipnis/ql_rest ql_rest
cd ql_rest
git checkout docker

./configure --with-deps-include=/usr/local/include/ --with-deps-lib=/usr/local/lib
make install
