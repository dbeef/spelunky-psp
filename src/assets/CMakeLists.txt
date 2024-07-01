project(Assets)

add_library(Assets STATIC
        interface/assets/Assets.hpp
        src/Assets.cpp
        interface/assets/Paths.hpp
        src/Paths.cpp
)

target_include_directories(Assets
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(Assets PROPERTIES
        CXX_STANDARD 17
)

target_link_libraries(Assets
        PUBLIC
        Patterns
        PRIVATE
        Logger
)
