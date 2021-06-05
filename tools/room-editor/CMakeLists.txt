add_executable(RoomEditor src/Main.cpp include/MapTileToString.hpp include/Serialize.hpp include/Deserialize.hpp include/HeaderFile.hpp)
target_link_libraries(RoomEditor PRIVATE raylib Level)
set_target_properties(RoomEditor PROPERTIES CXX_STANDARD 14)
target_include_directories(RoomEditor PRIVATE include)
