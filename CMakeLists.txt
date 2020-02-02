cmake_minimum_required(VERSION 3.10)
project(Spelunky_PSP)

include(cmake/CurrentPlatform.cmake)

spelunky_psp_detect_platform()
spelunky_psp_add_platform_dependencies()

if (SPELUNKY_PSP_PLATFORM_LINUX)
    # Tools meant to be run only on host (development) system.
    add_subdirectory(tools)
endif()

set(ASSETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets)

add_executable(Spelunky_PSP src/Main.cpp)

add_subdirectory(src/video)
add_subdirectory(src/video/graphics_utils)
add_subdirectory(vendor)
add_subdirectory(src/level_generator)
add_subdirectory(src/level_renderer)
add_subdirectory(src/camera)
add_subdirectory(src/input)

target_include_directories(Spelunky_PSP PRIVATE src)

target_link_libraries(Spelunky_PSP PRIVATE Logger Video Dependencies Camera LevelGenerator LevelRenderer Input)

set_target_properties(Spelunky_PSP
        PROPERTIES
        CXX_STANDARD
        14
        CXX_EXTENSIONS
        OFF
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

install(TARGETS Spelunky_PSP
        CONFIGURATIONS Debug
        RUNTIME DESTINATION Debug/bin)

install(TARGETS Spelunky_PSP
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin)

spelunky_psp_post_build()
