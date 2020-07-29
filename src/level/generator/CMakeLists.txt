project(Level)

add_library(Level STATIC
        src/Level.cpp
        src/MapTile.cpp
        src/TileBatch.cpp
        interface/Level.hpp
        interface/MapTile.hpp
        interface/MapTileType.hpp
        interface/SplashScreenType.hpp
        interface/TileOrientation.hpp
        interface/TileBatch.hpp
        )

target_include_directories(Level
        PRIVATE include interface
        INTERFACE interface
        )

set_target_properties(Level PROPERTIES
        CXX_STANDARD
        14
        )

target_link_libraries(Level
        PUBLIC
            Renderer
        PRIVATE
            Rooms
            glad
            TextureBank
        )
