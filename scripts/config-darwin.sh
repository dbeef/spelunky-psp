#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

INSTALL_PATH=`pwd`/tmp/install-darwin

rm -rf tmp/build-darwin
mkdir -p tmp/build-darwin
cd tmp/build-darwin

cmake ../../ \
-G "Unix Makefiles" \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
