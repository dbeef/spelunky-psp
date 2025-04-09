project(Other)

add_library(Other STATIC
        interface/other/LootCollectedEvent.hpp
        interface/other/NpcType.hpp
        interface/other/Inventory.hpp
        interface/other/InventoryEvent.hpp
        interface/other/ParticleGenerator.hpp
        src/other/Inventory.cpp
        src/other/ParticleGenerator.cpp
        src/other/NpcType.cpp
        src/other/ItemType.cpp
        src/other/CheatConsoleInterpreter.cpp
)

target_include_directories(Other
        PRIVATE interface interface
        INTERFACE interface
)

set_target_properties(Other PROPERTIES
        CXX_STANDARD
        17
)

target_link_libraries(Other
        PUBLIC
        Viewport
        Patterns
        EntityRegistry
        RenderingTypes
        Level
        MainDude
        Prefabs
        PRIVATE
        Logger
)

if (SPELUNKY_PSP_PLATFORM_LINUX)
    target_link_libraries(Other PRIVATE dl)
    target_compile_definitions(Other PRIVATE SPELUNKY_PSP_PLATFORM_LINUX)
elseif (SPELUNKY_PSP_PLATFORM_PSP)
    target_compile_definitions(Other PRIVATE SPELUNKY_PSP_PLATFORM_PSP)
endif ()
