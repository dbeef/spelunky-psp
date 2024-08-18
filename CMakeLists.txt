cmake_minimum_required(VERSION 3.10)
project(Spelunky_PSP)

include(cmake/CurrentPlatform.cmake)

option(SPELUNKY_PSP_USE_VIRTUAL_FILESYSTEM "Whether to resource-compile assets within the executable" ON)

spelunky_psp_detect_platform()
spelunky_psp_add_platform_dependencies()

set(SPELUNKY_PSP_ASSETS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/assets)

if (SPELUNKY_PSP_PLATFORM_ANDROID)
    # On Android, libSpelunky_PSP.so is copied into the final .APK file and SDL2's Java activity
    # calls the main function through JNI.
    add_library(Spelunky_PSP SHARED src/Main.cpp)
else()
    add_executable(Spelunky_PSP src/Main.cpp)
endif ()

add_subdirectory(vendor)
add_subdirectory(src/assets)
add_subdirectory(src/entity-registry)
add_subdirectory(src/viewport)
add_subdirectory(src/video)
add_subdirectory(src/audio)
add_subdirectory(src/level)
add_subdirectory(src/rendering-types)
add_subdirectory(src/time)
add_subdirectory(src/camera)
add_subdirectory(src/input)
add_subdirectory(src/graphics-utils)
add_subdirectory(src/game-loop)
add_subdirectory(src/texture-bank)
add_subdirectory(src/collisions)
add_subdirectory(src/patterns)

target_link_libraries(Spelunky_PSP PRIVATE
        Assets
        Logger
        Video
        Audio
        Level
        RenderingTypes
        Input
        GameLoop
        TextureBank
        Dependencies
)

set_target_properties(Spelunky_PSP
        PROPERTIES
        CXX_STANDARD
        17
        CXX_EXTENSIONS
        OFF
)

if (SPELUNKY_PSP_PLATFORM_LINUX OR
    SPELUNKY_PSP_PLATFORM_DARWIN OR
    SPELUNKY_PSP_PLATFORM_PSP OR
    SPELUNKY_PSP_PLATFORM_ANDROID)
        set_target_properties(Spelunky_PSP
                PROPERTIES
                COMPILE_FLAGS
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
        RUNTIME DESTINATION Debug/bin
        LIBRARY DESTINATION Debug/lib
)

install(TARGETS Spelunky_PSP
        CONFIGURATIONS Release
        RUNTIME DESTINATION Release/bin
        LIBRARY DESTINATION Release/lib
)

spelunky_psp_post_build()
