# That file is here only to make IDE's happy so they could index all source files.
# Real building happens through Makefile.

cmake_minimum_required(VERSION 3.9)
project(spelunkypsp)

set(CMAKE_CXX_STANDARD 98)

# Set to your psptoolchain installation dir if environment variable does not exist.
set(PSPDEV $ENV{PSPDEV})

include_directories(${PSPDEV}/include)
include_directories(${PSPDEV}/psp/include)
include_directories(${PSPDEV}/psp/sdk/include)

# Using makefile for building but adding target here in CMakeLists since apparently
# clion won't scan files without it.
add_executable(spelunkypsp
        input/InputHandler.h input/InputHandler.cpp main.cpp camera/Camera.cpp camera/Camera.h
        timer/Timer.cpp timer/Timer.h utils/Consts.h tiles/MapTileType.h
        rooms/AltarRoom.hpp rooms/ClosedRooms.hpp rooms/EntranceRooms.hpp rooms/ExitRooms.hpp
        rooms/LeftRightDownRooms.hpp rooms/LeftRightRooms.hpp rooms/LeftRightUpRooms.hpp rooms/RoomType.hpp
        rooms/ShopRooms.hpp rooms/SnakeWellRoom.hpp rooms/SplashScreenRooms.hpp
        )