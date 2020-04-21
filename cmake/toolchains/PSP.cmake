#
# CMake Toolchain for PSP
#
#
# Copyright 2019 - Wally

# Source:
# https://github.com/z2442/daedalus
# Contains small modifications.

# Basic CMake Declarations - Required (CMAKE_SYSTEM_NAME sets cross compiling to true)
set(CMAKE_SYSTEM_NAME Generic)
set(TOOLCHAIN "${PSPDEV}")
set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})
set(CMAKE_FIND_ROOT_PATH "${PSPDEV}/psp/sdk;${PSPDEV}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Get the parent PSPDev Directory to determine where the SDK is hiding
execute_process(COMMAND bash -c "psp-config --pspdev-path" OUTPUT_VARIABLE PSPDEV OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set variables for PSPSDK / Bin
set(PSPSDK ${PSPDEV}/psp/sdk)
set(PSPBIN ${PSPDEV}/bin/)

# Include Directories
include_directories(${include_directories} .. ${PSPSDK}/include)

add_definitions("-G0")
add_definitions("-D__PSP__")
add_definitions("-DHAVE_OPENGL")

# Set Variables for PSPSDK aplications
set(CC ${PSPBIN}psp-gcc)
set(CXX ${PSPBIN}psp-g++)
set(AS ${PSPBIN}psp-as) ##This was psp-gcc in the build.mak
set(LD ${PSPBIN}psp-gcc) ##This was psp-gcc in the build.mak
set(AR ${PSPBIN}psp-ar)
set(RANLIB ${PSPBIN}psp-ranlib)
set(STRIP1 ${PSPBIN}psp-strip) ##can't use STRIP as it's a variable in CMake
set(MKSFO ${PSPBIN}mksfo)
set(PACK_PBP ${PSPBIN}pack-pbp)
set(FIXUP ${PSPBIN}psp-fixup-imports)
set(ENC ${PSPBIN}PrxEncrypter)

# SpelunkyPSP specific

set(SPELUNKY_PSP_PLATFORM PSP)
set(SPELUNKY_PSP_PLATFORM_PSP TRUE)

# Assert that PSP SDK path has been actually passed:
if (NOT DEFINED PSPDEV)
    message(FATAL_ERROR "PSPDEV not defined. Pass -DPSPDEV=/usr/local/pspdev or other installation location.")
endif ()

# set find root path
execute_process(COMMAND psp-config --pspsdk-path
        OUTPUT_VARIABLE PSPSDK_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND psp-config --psp-prefix
        OUTPUT_VARIABLE PSP_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE)

set(CMAKE_FIND_ROOT_PATH "${PSPSDK_PATH};${PSP_PREFIX}")

set(PSP_LIBRARIES
        "-lpspdebug -lpspdisplay -lpspge -lpspctrl -lc -lpspsdk -lc \
        -lpspnet -lpspnet_inet -lpspnet_apctl -lpspnet_resolver -lpspaudiolib \
        -lpsputility -lpspuser -lpspkernel -L${PSPSDK_PATH}/lib -L${PSP_PREFIX}/lib"
)

set(CMAKE_C_STANDARD_LIBRARIES "${PSP_LIBRARIES}")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lstdc++ ${PSP_LIBRARIES} ")
