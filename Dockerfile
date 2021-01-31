FROM ubuntu:20.10

# Docker image with toolchains/dependencies for the following platforms:
#   * Linux
#   * PSP
#   * Android
#
# TODO: Split this Dockerfile into 3 separate ones, as handling that much platforms
#       is too much competence and the final image grows larger.

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        g++ \
        build-essential \
        autoconf \
        automake \
        cmake \
        doxygen \
        bison \
        flex \
        libncurses5-dev \
        libsdl1.2-dev \
        libsdl-mixer1.2-dev \
        libsdl2-dev \
        libreadline-dev \
        libusb-dev \
        texinfo \
        libgmp3-dev \
        libmpfr-dev \
        libelf-dev \
        libmpc-dev \
        libfreetype6-dev \
        zlib1g-dev \
        libtool \
        libtool-bin \
        subversion \
        git \
        tcl \
        unzip \
        wget \
        libcurl4-openssl-dev \
        libgpgme-dev \
        libarchive-dev \
        libssl-dev \
    && apt-get clean

RUN git clone https://github.com/pspdev/psptoolchain.git --depth 1

WORKDIR psptoolchain

RUN ./toolchain-sudo.sh && ./toolchain-sudo.sh clean && rm -rf build

# Set environment variables
RUN export PSPDEV=/usr/local/pspdev
RUN export PATH=$PATH:$PSPDEV/bin
ENV PSPDEV /usr/local/pspdev
ENV PATH $PATH:$PSPDEV/bin

# own requirements
RUN apt-get install libglew-dev -y

# Android NDK:

RUN mkdir /usr/local/android/
WORKDIR /usr/local/android
RUN wget https://dl.google.com/android/repository/android-ndk-r21e-linux-x86_64.zip --no-check-certificate
RUN unzip android-ndk-r21e-linux-x86_64.zip
RUN rm android-ndk-r21e-linux-x86_64.zip

RUN export ANDROID_NDK_HOME=/usr/local/android/android-ndk-r21e
ENV ANDROID_NDK_HOME /usr/local/android/android-ndk-r21e

# Android SDK:

RUN apt-get update && apt-get install -y libncurses5 openjdk-8-jdk-headless

RUN mkdir cmdline-tools 
WORKDIR cmdline-tools
RUN wget https://dl.google.com/android/repository/commandlinetools-linux-6609375_latest.zip --no-check-certificate
RUN unzip commandlinetools-linux-6609375_latest.zip
RUN rm commandlinetools-linux-6609375_latest.zip
WORKDIR tools/bin
RUN yes | ./sdkmanager "platforms;android-26"
RUN yes | ./sdkmanager "platform-tools"
RUN yes | ./sdkmanager "build-tools;29.0.2"
RUN yes | ./sdkmanager "cmake;3.10.2.4988404"

RUN export ANDROID_HOME=/usr/local/android
ENV ANDROID_HOME /usr/local/android

WORKDIR /usr/local/android

# Cross-compile SDL2 for armeabi-v7a, arm64-v8a:

RUN mkdir -p deps-armeabi-v7a/SDL2
RUN mkdir -p deps-arm64-v8a/SDL2

RUN export ANDROID_DEPS_ARM64_V8A=/usr/local/android/deps-arm64-v8a
RUN export ANDROID_DEPS_ARMEABI_V7A=/usr/local/android/deps-armeabi-v7a

ENV ANDROID_DEPS_ARM64_V8A /usr/local/android/deps-arm64-v8a
ENV ANDROID_DEPS_ARMEABI_V7A /usr/local/android/deps-armeabi-v7a

RUN wget https://www.libsdl.org/release/SDL2-2.0.14.tar.gz --no-check-certificate
RUN tar -xzf SDL2-2.0.14.tar.gz
RUN rm SDL2-2.0.14.tar.gz

WORKDIR /usr/local/android/SDL2-2.0.14
RUN mkdir build-armeabi-v7a
WORKDIR build-armeabi-v7a
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
             -DCMAKE_INSTALL_PREFIX=$ANDROID_DEPS_ARMEABI_V7A/SDL2 \
             -DSDL_STATIC=ON \
             -DSDL_SHARED=OFF \
             -DANDROID_ABI=armeabi-v7a \
             -DANDROID_PLATFORM=android-15
RUN cmake --build . --target install -j 4

WORKDIR /usr/local/android/SDL2-2.0.14
RUN mkdir build-arm64-v8a
WORKDIR build-arm64-v8a
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
             -DCMAKE_INSTALL_PREFIX=$ANDROID_DEPS_ARM64_V8A/SDL2 \
             -DSDL_STATIC=ON \
             -DSDL_SHARED=OFF \
             -DANDROID_ABI=arm64-v8a \
             -DANDROID_PLATFORM=android-15
RUN cmake --build . --target install -j 4

WORKDIR /usr/local/android
RUN rm -r SDL2-2.0.14

# Reset working directory:

WORKDIR /usr/local
