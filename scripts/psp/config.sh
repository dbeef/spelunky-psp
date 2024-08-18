#/usr/bin/env bash

source variables.sh

# "psp-cmake" is guaranteed to be in PATH after successful pspdev/psptoolchain installation AND in the official PSP SDK Docker image;
# it's a shell script calling host's vanilla cmake with cmake toolchain file already passed:

psp-cmake \
    -S $ROOT_PATH \
    -B $BUILD_PATH \
    -DSPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH \
    -DCMAKE_VERBOSE_MAKEFILE=ON
