project(Time)

add_library(Time STATIC
        src/Timestep.cpp
        interface/time/Timestep.hpp
)

target_include_directories(Time
        PRIVATE include interface
        INTERFACE interface
)

target_link_libraries(Time
    PUBLIC
        SDL_2_XX)

set_target_properties(Time PROPERTIES
        CXX_STANDARD
        14
)
