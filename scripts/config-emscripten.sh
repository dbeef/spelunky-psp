#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

INSTALL_PATH=`pwd`/tmp/install-emscripten
EMSCRIPTEN_ROOT=$EMSDK/upstream/emscripten

rm -rf tmp/build-emscripten
mkdir -p tmp/build-emscripten
cd tmp/build-emscripten

cmake -S ../../ \
-G "Unix Makefiles" \
-DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN_ROOT/cmake/Modules/Platform/Emscripten.cmake \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
