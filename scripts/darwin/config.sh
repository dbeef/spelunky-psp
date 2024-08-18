#!/usr/bin/env bash

source variables.sh

cmake \
    -B $BUILD_PATH \
    -S $ROOT_PATH \
    -G "Unix Makefiles" \
    -DSPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
