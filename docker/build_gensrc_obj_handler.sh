#!/bin/bash

DEPS_BUILD_DIR=/ql_rest_deps_build

mkdir -p $DEPS_BUILD_DIR

GENSRC_PKG=gensrc-1.22.0
OBJECTHANDLER_PKG=ObjectHandler-1.22.0

[[ ! -f $DEPS_BUILD_DIR/$GENSRC_PKG.tar.gz ]] && curl -L "https://github.com/eehlers/QuantLibAddin-Old/files/6415426/$GENSRC_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$GENSRC_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/$OBJECTHANDLER_PKG.tar.gz ]] && curl -L "https://github.com/eehlers/QuantLibAddin-Old/files/6415431/$OBJECTHANDLER_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$OBJECTHANDLER_PKG.tar.gz

cd $DEPS_BUILD_DIR
tar xvf $GENSRC_PKG.tar.gz
tar xvf $GENSRC_PKG.tar.gz
cd $GENSRC_PKG
./configure CXXFLAGS="-O2" --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR 
make install


cd $DEPS_BUILD_DIR
tar xvf $OBJECTHANDLER_PKG.tar.gz
cd $OBJECTHANDLER_PKG
./configure CXXFLAGS="-O2" --with-gensrc=$DEPS_BUILD_DIR/gensrc-1.22.0 --without-log4cxx
make install
