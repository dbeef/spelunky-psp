project(LevelGenerator)

add_library(LevelGenerator STATIC
        src/LevelGenerator.cpp
        interface/LevelGenerator.hpp
        interface/MapTile.hpp interface/MapTileType.hpp src/MapTile.cpp interface/SplashScreenType.hpp interface/TileOrientation.hpp interface/Direction.hpp interface/Level.hpp src/Level.cpp)

target_include_directories(LevelGenerator
        PRIVATE include interface
        INTERFACE interface
)

set_target_properties(LevelGenerator PROPERTIES
        CXX_STANDARD
        14
)

target_link_libraries(LevelGenerator PRIVATE Rooms Camera)
