#!/usr/bin/env bash

source variables.sh

cmake \
    --build $BUILD_PATH \
    --target install \
    --config Release \
    -j $NUM_THREADS
