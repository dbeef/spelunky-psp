project(LevelRenderer)

add_library(LevelRenderer STATIC
        src/LevelRenderer.cpp
        interface/LevelRenderer.hpp
)

target_include_directories(LevelRenderer
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(LevelRenderer PROPERTIES
        CXX_STANDARD
        14
)
