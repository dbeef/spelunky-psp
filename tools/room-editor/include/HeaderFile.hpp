#pragma once

#include "Serialize.hpp"
#include "MapTileType.hpp"

#include <string>
#include <utility>
#include <vector>
#include <array>

using Room = std::array<std::array<MapTileType, 10>, 10>;

class HeaderFile
{
public:

    explicit HeaderFile(const std::string& filePath)
        : _filePath(filePath)
        , _fileNamespace(get_namespace_out_of_filename(_filePath))
    {
    }

    const std::vector<Room>& get_rooms() const { return _rooms; }
    const std::string& get_file_namespace() const { return _fileNamespace; }
    void add_room(const Room& room) { _rooms.push_back(room); }

private:
    std::vector<Room> _rooms;
    const std::string _filePath;
    const std::string _fileNamespace;
};
