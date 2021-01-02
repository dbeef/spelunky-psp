project(Camera)

add_library(Camera STATIC
    src/ModelViewCamera.cpp
    src/ScreenSpaceCamera.cpp
    interface/ModelViewCamera.hpp
    interface/ScreenSpaceCamera.hpp
)

target_include_directories(Camera
    PRIVATE include interface
    INTERFACE interface
)

set_target_properties(Camera PROPERTIES
    CXX_STANDARD
    17
)

target_link_libraries(Camera
    PUBLIC
        Viewport
    PRIVATE
        GraphicsUtils
        Logger
    )
