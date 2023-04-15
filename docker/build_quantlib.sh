#!/bin/bash

DEPS_BUILD_DIR=/ql_rest_deps_build

mkdir -p $DEPS_BUILD_DIR

QUANTLIB_PKG=QuantLib-1.22

[[ ! -f $DEPS_BUILD_DIR/$QUANTLIB_PKG.tar.gz ]] && curl -L "https://github.com/lballabio/QuantLib/releases/download/QuantLib-v1.22/$QUANTLIB_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$QUANTLIB_PKG.tar.gz

cd $DEPS_BUILD_DIR
tar xvf $QUANTLIB_PKG.tar.gz
cd $QUANTLIB_PKG
./configure  CXXFLAGS="-O2"
make install
