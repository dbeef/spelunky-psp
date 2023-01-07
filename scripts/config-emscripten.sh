#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

INSTALL_PATH=`pwd`/tmp/install-darwin
EMSCRIPTEN_ROOT=/opt/homebrew/Cellar/emscripten/3.1.28

rm -rf tmp/build-darwin
mkdir -p tmp/build-darwin
cd tmp/build-darwin

cmake -S ../../ \
-G "Unix Makefiles" \
-DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN_ROOT/libexec/cmake/Modules/Platform/Emscripten.cmake \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
