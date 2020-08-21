#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

source scripts/variables-miyoo.sh

rm -rf $INSTALL_PATH
mkdir -p $INSTALL_PATH

rm -rf $BUILD_PATH
mkdir -p $BUILD_PATH

cd $BUILD_PATH

TOOLCHAIN_FILE_PATH=$ROOT_PATH/cmake/MiyooCFW.cmake

cmake -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE_PATH $ROOT_PATH \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH \
-DCMAKE_VERBOSE_MAKEFILE=ON
