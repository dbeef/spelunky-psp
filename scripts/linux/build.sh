#!/usr/bin/env bash

source variables.sh

cmake \
    --build $SPELUNKY_PSP_BUILD_PATH \
    --target install \
    --config Release \
    -j $NUM_THREADS
