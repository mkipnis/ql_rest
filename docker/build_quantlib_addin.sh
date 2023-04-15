#!/bin/bash

DEPS_BUILD_DIR=/ql_rest_deps_build

mkdir -p $DEPS_BUILD_DIR

QUANTLIBADDIN_PKG=QuantLibAddin-1.22.0

[[ ! -f $DEPS_BUILD_DIR/$QUANTLIBADDIN_PKG.tar.gz ]] && curl -L "https://github.com/eehlers/QuantLibAddin-Old/files/6415434/$QUANTLIBADDIN_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$QUANTLIBADDIN_PKG.tar.gz

cd $DEPS_BUILD_DIR
tar xvf $QUANTLIBADDIN_PKG.tar.gz
cd $QUANTLIBADDIN_PKG
./configure CXXFLAGS="-DWAZOO_64_BIT"  --with-gensrc=$DEPS_BUILD_DIR/gensrc-1.22.0 --enable-addin-cpp --with-oh=$DEPS_BUILD_DIR/ObjectHandler-1.22.0
make install -j 10
