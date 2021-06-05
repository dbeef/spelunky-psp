#pragma once

#include <ostream>
#include "MapTileType.hpp"

void serialize(std::ostream& out, MapTileType room[10][10])
{
    out << "// This is a generated room layout - To edit it, you can use RoomEditor dev-tool\n"
        << "#pragma once\n"
        << "static const int rooms[10][10] = \n"
        << "{\n";

    for (std::size_t x_index = 0; x_index < 10; x_index++)
    {
        out << "{";
        for (std::size_t y_index = 0; y_index < 10; y_index++)
        {
            out << static_cast<std::size_t>(room[x_index][y_index]) << ", ";
        }
        out << "},\n";
    }

    out << "};\n";
}
