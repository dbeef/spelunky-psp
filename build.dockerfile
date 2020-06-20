FROM ubuntu:20.10

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
