#!/bin/bash

set -x 

CURRENT_DIR=`pwd`
DEPS_BUILD_DIR=$HOME/ql_rest_deps_build

mkdir -p $DEPS_BUILD_DIR 

if [[ "$OSTYPE" == "darwin"* ]]; then
	CXXFLAGS="-DWAZOO_64_BIT -std=c++11 -stdlib=libc++"
fi

if [ -z "$1" ] 
then
    INSTALL_DIR=$DEPS_BUILD_DIR
    echo "Install directory is not supplied, installing in $INSTALL_DIR/"
else
    INSTALL_DIR=$1
fi

BOOST_PKG=boost_1_80_0
QUANTLIB_PKG=QuantLib-1.22
GENSRC_PKG=gensrc-1.22.0
OBJECTHANDLER_PKG=ObjectHandler-1.22.0
QUANTLIBADDIN_PKG=QuantLibAddin-1.22.0

[[ ! -f $DEPS_BUILD_DIR/$BOOST_PKG.tar.gz ]] && curl -L "https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/$BOOST_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$BOOST_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/$QUANTLIB_PKG.tar.gz ]] && curl -L "https://github.com/lballabio/QuantLib/releases/download/QuantLib-v1.22/$QUANTLIB_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$QUANTLIB_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/$GENSRC_PKG.tar.gz ]] && curl -L "https://github.com/eehlers/QuantLibAddin-Old/files/6415426/$GENSRC_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$GENSRC_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/$OBJECTHANDLER_PKG.tar.gz ]] && curl -L "https://github.com/eehlers/QuantLibAddin-Old/files/6415431/$OBJECTHANDLER_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$OBJECTHANDLER_PKG.tar.gz
[[ ! -f $DEPS_BUILD_DIR/$QUANTLIBADDIN_PKG.tar.gz ]] && curl -L "https://github.com/eehlers/QuantLibAddin-Old/files/6415434/$QUANTLIBADDIN_PKG.tar.gz"  -o $DEPS_BUILD_DIR/$QUANTLIBADDIN_PKG.tar.gz

if [[ ! -f $INSTALL_DIR/include/boost/version.hpp ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d $BOOST_PKG ]] && tar xvf $BOOST_PKG.tar.gz
cd $BOOST_PKG
./bootstrap.sh --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR
./b2 install

fi

if [[ ! -f $INSTALL_DIR/include/ql/version.hpp ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d $QUANTLIB_PKG ]] && tar xvf $QUANTLIB_PKG.tar.gz
cd $QUANTLIB_PKG
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR --with-boost-include=$INSTALL_DIR/include --with-boost-lib=$INSTALL_DIR/lib CXXFLAGS="$CXXFLAGS"
make -j 10 install 

fi


cd $DEPS_BUILD_DIR
[[ ! -d $GENSRC_PKG ]] && tar xvf $GENSRC_PKG.tar.gz
tar xvf $GENSRC_PKG.tar.gz
cd $GENSRC_PKG
make clean
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR 


if [[ ! -d $INSTALL_DIR/include/oh ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d $OBJECTHANDLER_PKG ]] && tar xvf $OBJECTHANDLER_PKG.tar.gz
cd $OBJECTHANDLER_PKG
make clean
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR CXXFLAGS="$CXXFLAGS -I$INSTALL_DIR/include" LDFLAGS="-L$INSTALL_DIR/lib" --with-gensrc=$DEPS_BUILD_DIR/gensrc-1.22.0 --without-log4cxx
make -j 10 install

fi

if [[ ! -d $INSTALL_DIR/include/qlo ]]
then

cd $DEPS_BUILD_DIR
[[ ! -d $QUANTLIBADDIN_PKG ]] && tar xvf $QUANTLIBADDIN_PKG.tar.gz
cd $QUANTLIBADDIN_PKG
make clean
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR  CXXFLAGS="$CXXFLAGS -I$INSTALL_DIR/include" LDFLAGS="-L$INSTALL_DIR/lib" --with-gensrc=$DEPS_BUILD_DIR/gensrc-1.22.0 --enable-addin-cpp --with-oh=$DEPS_BUILD_DIR/ObjectHandler-1.22.0
make -j 10 install

fi

cd $CURRENT_DIR
./configure --prefix=$INSTALL_DIR --exec-prefix=$INSTALL_DIR  CXXFLAGS="$CXXFLAGS -I$INSTALL_DIR/include"
make install
