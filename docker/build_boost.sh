#!/bin/bash

CURRENT_DIR=`pwd`
DEPS_BUILD_DIR=/ql_rest_deps_build

mkdir -p $DEPS_BUILD_DIR

BOOST_PKG=boost_1_80_0

[[ ! -f $DEPS_BUILD_DIR/$BOOST_PKG.tar.gz ]] && curl -L "https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/$BOOST_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$BOOST_PKG.tar.gz

cd $DEPS_BUILD_DIR
tar xvf $BOOST_PKG.tar.gz
cd $BOOST_PKG
./bootstrap.sh
./b2 install
