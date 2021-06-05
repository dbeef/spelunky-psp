#pragma once

#include <ostream>
#include <iostream>
#include <string>

#include "MapTileType.hpp"
#include "HeaderFile.hpp"

std::string get_namespace_out_of_filename(const std::string& filename)
{
    std::string out = filename;

    std::size_t dotPosition = std::string::npos;
    std::size_t slashPosition = std::string::npos;

    for (std::size_t index = filename.size() - 1; index >= 0; index--)
    {
        if (out[index] == '.')
        {
            out[index] = '_';
            dotPosition = index;
        }
        else if (out[index] == '/')
        {
            slashPosition = index;
        }

        if (dotPosition != std::string::npos && slashPosition != std::string::npos)
        {
            break;
        }
    }

    return out.substr(slashPosition + 1, dotPosition);
}

void serialize(std::ostream& out, const HeaderFile& header)
{
    if (header.rooms.empty())
    {
        std::cout << "Nothing to serialize" << std::endl;
    }

    out << "// This is a generated room layout - To edit it, you can use RoomEditor dev-tool\n"
        << "#pragma once\n"
        << "namespace " << header.fileNamespace << "\n{\n"
        << "static const int rooms[" << header.rooms.size() << "][10][10] = \n"
        << "{\n";

    for (const auto& room : header.rooms)
    {
        out << "{\n";
        for (std::size_t x_index = 0; x_index < 10; x_index++)
        {
            out << "{";
            for (std::size_t y_index = 0; y_index < 10; y_index++)
            {
                out << static_cast<std::size_t>(room[x_index][y_index]) << ", ";
            }
            out << "},\n";
        }
        out << "},\n";
    }

    out << "};\n}\n";
}
