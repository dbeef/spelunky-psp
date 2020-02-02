project(Time)

add_library(Time STATIC
        src/Timestep.cpp
        interface/time/Timestep.hpp
)

target_include_directories(Time
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Time PROPERTIES
        CXX_STANDARD
        14
)
