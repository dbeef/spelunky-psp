project(Collisions)

add_library(Collisions STATIC
        interface/Collisions.hpp
        src/Collisions.cpp
        )

target_include_directories(Collisions
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(Collisions PROPERTIES
        CXX_STANDARD
        17
        )

target_link_libraries(Collisions PRIVATE
        Logger
        Level
        )
