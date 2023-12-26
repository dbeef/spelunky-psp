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
        include/AltarRoom.hpp
        include/ClosedRooms.hpp
        include/EntranceRooms.hpp
        include/ExitRooms.hpp
        include/LeftRightDownRooms.hpp
        include/LeftRightRooms.hpp
        include/LeftRightUpRooms.hpp
        include/ShopRooms.hpp
        include/SplashScreenRooms.hpp
        interface/NPCType.hpp
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
        PRIVATE
            glad
            TextureBank
        )
