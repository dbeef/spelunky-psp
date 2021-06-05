add_executable(RoomEditor src/Main.cpp)
target_link_libraries(RoomEditor PRIVATE raylib Level)
set_target_properties(RoomEditor PROPERTIES CXX_STANDARD 14)
