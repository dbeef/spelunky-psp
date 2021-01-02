project(EntityRegistry)

add_library(EntityRegistry STATIC
        interface/EntityRegistry.hpp
        src/EntityRegistry.cpp
        )

target_include_directories(EntityRegistry
        PRIVATE interface
        INTERFACE interface
        )

set_target_properties(EntityRegistry PROPERTIES
        CXX_STANDARD
        17
        )

target_link_libraries(EntityRegistry
        PUBLIC
        entt
        Patterns
        )
