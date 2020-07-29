project(LevelGenerator)

# FIXME: This project is very poorly written (copy-pasted from Spelunky-NDS), needs refactoring.

add_library(LevelGenerator STATIC
        src/Level.cpp
        interface/Level.hpp
        interface/MapTile.hpp
        interface/MapTileType.hpp
        src/MapTile.cpp
        interface/SplashScreenType.hpp
        interface/TileOrientation.hpp
        interface/Direction.hpp
        interface/TileBatch.hpp
        src/TileBatch.cpp
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
