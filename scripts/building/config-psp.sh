#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

source scripts/building/variables-psp.sh

rm -rf $INSTALL_PATH
mkdir -p $INSTALL_PATH

rm -rf $BUILD_PATH
mkdir -p $BUILD_PATH

cd $BUILD_PATH

# psp-cmake is guaranteed to be in path after successful pspdev/psptoolchain installation;
# it's a shell script calling host's vanilla cmake with cmake toolchain file already passed.

psp-cmake $ROOT_PATH \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
