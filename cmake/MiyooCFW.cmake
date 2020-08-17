#
# CMake toolchain file for PSP.
#
# Copyright 2019 - Wally
# Copyright 2020 - Daniel 'dbeef' Zalega

set(MiyooCFW_PATH "/home/dbeef/miyoo-toolchain/")
set(MiyooBIN "${MiyooCFW_PATH}/bin/")

# Basic CMake Declarations
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${MiyooBIN}/arm-linux-gcc)
set(CMAKE_CXX_COMPILER ${MiyooBIN}/arm-linux-g++)
set(CMAKE_FIND_ROOT_PATH "${MiyooCFW_PATH}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(STRIP ${MiyooBIN}/arm-linux-strip)

# Include directories:
include_directories(${include_directories} ${MiyooCFW_PATH}/include)

# Helper variable for multi-platform projects to identify current platform:
set(Miyoo_PLATFORM TRUE BOOL)
