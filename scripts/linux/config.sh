#!/usr/bin/env bash

source variables.sh

cmake \
    -S $ROOT_PATH \
    -B $BUILD_PATH \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
