#!/usr/bin/env bash

source variables.sh

cmake \
    -B $BUILD_PATH \
    -S $ROOT_PATH \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH
