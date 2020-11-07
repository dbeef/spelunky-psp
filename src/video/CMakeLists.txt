project(Video)

add_library(Video STATIC
        interface/video/Video.hpp
        src/Video.cpp
        src/Video_GLFBDEV.cpp
)

target_include_directories(Video
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Video PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(Video
    PUBLIC
        Time
        Viewport
        /home/dbeef/MiyooSDK/mesa/lib/libOSMesa.so.7.9.0
        /home/dbeef/MiyooSDK/mesa/lib/libGL.so.7.9.0
    PRIVATE
        Logger
        glad
        Input
        GraphicsUtils
)
