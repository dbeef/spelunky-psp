project(Patterns)

add_library(Patterns STATIC
        interface/patterns/Singleton.hpp
        src/Singleton.cpp
        )

target_include_directories(Patterns
        PRIVATE interface
        INTERFACE interface
        )

set_target_properties(Patterns PROPERTIES
        CXX_STANDARD
        14
        )
