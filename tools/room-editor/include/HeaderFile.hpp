#pragma once

#include "MapTileType.hpp"
#include <string>
#include <vector>
#include <array>

using Room = std::array<std::array<MapTileType, 10>, 10>;

struct HeaderFile
{
    std::vector<Room> rooms;
    std::string fileNamespace;
    std::string filePath;
};
