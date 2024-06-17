#!/usr/bin/env bash

source variables.sh

cmake \
    -S $ROOT_PATH \
    -B $SPELUNKY_PSP_BUILD_PATH \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$SPELUNKY_PSP_INSTALL_PATH
