#!/usr/bin/env bash
# This script is meant to be run from the root of the project.

source scripts/variables-android.sh

rm -rf $INSTALL_PATH
mkdir -p $INSTALL_PATH

rm -rf $BUILD_PATH
mkdir -p $BUILD_PATH

cd $BUILD_PATH

# Following variables must be present in the environment:
# ANDROID_NDK_HOME - path to the Android NDK, i.e /opt/android/android-ndk-r19c
# ANDROID_DEPS - path to libraries cross-compiled specifically for your target architecture,
#                i.e /opt/android/libs-armeabi-v7a

cmake $ROOT_PATH \
	-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
	-DCMAKE_PREFIX_PATH=$ANDROID_DEPS \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=$INSTALL_PATH \
	-DANDROID_ABI=armeabi-v7a \
	-DANDROID_PLATFORM=16 \
	-DSDL2_DIR=$ANDROID_DEPS/SDL2/lib/cmake/SDL2/
