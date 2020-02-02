project(Rooms)

add_library(Rooms STATIC
        src/Dummy.cpp
        interface/AltarRoom.hpp
        interface/ClosedRooms.hpp
        interface/EntranceRooms.hpp
        interface/ExitRooms.hpp
        interface/LeftRightDownRooms.hpp
        interface/LeftRightRooms.hpp
        interface/LeftRightUpRooms.hpp
        ../generator/interface/RoomType.hpp
        interface/ShopRooms.hpp
        interface/SnakeWellRoom.hpp
        interface/SplashScreenRooms.hpp
)

target_include_directories(Rooms INTERFACE interface)
set_target_properties(Rooms PROPERTIES CXX_STANDARD 14)
