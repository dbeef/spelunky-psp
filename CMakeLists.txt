cmake_minimum_required(VERSION 3.10)
project(Spelunky_PSP)

option(WITH_DEV_TOOLS "Tools meant to be run only on host (development) system." FALSE)

include(cmake/CurrentPlatform.cmake)

spelunky_psp_detect_platform()
spelunky_psp_add_platform_dependencies()

if (WITH_DEV_TOOLS)
    add_subdirectory(tools)
endif()

set(ASSETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_executable(Spelunky_PSP src/Main.cpp)

add_subdirectory(vendor)
add_subdirectory(src/viewport)
add_subdirectory(src/video)
add_subdirectory(src/level)
add_subdirectory(src/renderer)
add_subdirectory(src/time)
add_subdirectory(src/camera)
add_subdirectory(src/input)
add_subdirectory(src/graphics-utils)
add_subdirectory(src/game-loop)
add_subdirectory(src/texture-bank)
add_subdirectory(src/collisions)

target_link_libraries(Spelunky_PSP PRIVATE
        Logger
        Video
        Level
        Renderer
        Input
        GameLoop
        TextureBank
        Dependencies
)

set_target_properties(Spelunky_PSP
        PROPERTIES
        CXX_STANDARD
        14
        CXX_EXTENSIONS
        OFF
        )
if(SPELUNKY_PSP_PLATFORM_LINUX OR SPELUNKY_PSP_PLATFORM_PSP)
        set_target_properties(Spelunky_PSP
                PROPERTIES
                COMPILE_FLAGS
                # Treat every warning as an error
                -Werror
                # Disables exceptions support and uses the variant of C++ libraries without exceptions
                -fno-exceptions
                # Disable generation of information about every class with virtual functions for use by
                # the C++ runtime type identification features (dynamic_cast and typeid).
                -fno-rtti
                # Debugging information level. 0 is lowest.
                -G0
                # Optimize
                -O2
        )
endif()

install(TARGETS Spelunky_PSP
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)

install(TARGETS Spelunky_PSP
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)

spelunky_psp_post_build()
