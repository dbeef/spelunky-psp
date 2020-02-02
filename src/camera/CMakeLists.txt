project(Camera)

add_library(Camera STATIC
        src/Camera.cpp
        interface/Camera.hpp
)

target_include_directories(Camera
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Camera PROPERTIES
        CXX_STANDARD
        14
)
