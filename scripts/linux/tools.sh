#!/usr/bin/env bash

source variables.sh

cmake \
    -S $TOOLS_PATH \
    -B $TOOLS_BUILD_PATH \
    -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$TOOLS_INSTALL_PATH

# TODO: Add install target and pass --target install
cmake \
    --build $TOOLS_BUILD_PATH \
    --config Release \
    -j $NUM_THREADS
