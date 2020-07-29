project(LevelGenerator)

add_library(LevelGenerator STATIC
        src/Level.cpp
        src/MapTile.cpp
        src/TileBatch.cpp
        interface/Level.hpp
        interface/MapTile.hpp
        interface/MapTileType.hpp
        interface/SplashScreenType.hpp
        interface/TileOrientation.hpp
        interface/Direction.hpp
        interface/TileBatch.hpp
        )

target_include_directories(LevelGenerator
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(LevelGenerator PROPERTIES
        CXX_STANDARD
        14
        )

target_link_libraries(LevelGenerator
        PUBLIC
            Renderer
        PRIVATE
            Rooms
            glad
            TextureBank
        )
