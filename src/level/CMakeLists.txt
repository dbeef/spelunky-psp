project(Level)

add_library(Level STATIC
        src/Level.cpp
        src/MapTile.cpp
        src/TileBatch.cpp
        src/LootType.cpp
        interface/Level.hpp
        interface/MapTile.hpp
        interface/MapTileType.hpp
        interface/LootType.hpp
        interface/SplashScreenType.hpp
        interface/NeighbouringTiles.hpp
        interface/TileBatch.hpp
        interface/NPCType.hpp
        interface/LevelType.hpp
        interface/Stage.hpp
        include/generator/Pipeline.hpp
        src/generator/Pipeline.cpp
        src/generator/Stage.cpp
        include/generator/Pipeline.hpp
        src/generator/Pipeline.cpp

        src/populator/Populator.cpp
        src/populator/Shop.cpp
        include/populator/Populator.hpp
        include/populator/Spawner.hpp
        include/populator/Shop.hpp
        src/populator/ItemFactory.cpp
        src/populator/LootFactory.cpp
        src/populator/NpcFactory.cpp
        include/populator/ItemFactory.hpp
        include/populator/NpcFactory.hpp
        include/populator/LootFactory.hpp
        interface/RoomLayoutGenerator.hpp
        src/RoomLayoutGenerator.cpp
        src/generator/Spawners.cpp
        include/generator/Spawners.hpp
)

target_include_directories(Level
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(Level PROPERTIES
        CXX_STANDARD
        17
        )

target_link_libraries(Level
        PUBLIC
            entt
            RenderingTypes
            Patterns
            Prefabs
        PRIVATE
            glad
            TextureBank
            cjson
            Assets
            EntityRegistry
                Components
        )
