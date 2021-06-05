#include "Deserialize.hpp"
#include "MapTileType.hpp"

#include <fstream>
#include <cassert>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <cstdlib>

namespace
{
    std::vector<Room> saturate_rooms(const std::vector<std::size_t>& numbers)
    {
        std::vector<Room> rooms;
        std::size_t numbersIndex = 0;

        while (numbersIndex < numbers.size())
        {
            Room R;

            for (std::size_t y_index = 0; y_index < 10; y_index++)
            {
                for (std::size_t x_index = 0; x_index < 10; x_index++)
                {
                    R[x_index][y_index] = static_cast<MapTileType>(numbers.at(numbersIndex));
                    numbersIndex++;

                    //if (numbersIndex >= numbers.size())
                    //{
                    //    return rooms;
                    //}
                }
            }

            rooms.push_back(R);
        }

        return rooms;
    }

    std::vector<char> load_file_contents(const std::string &filePath)
    {
        std::ifstream headerFileStream(filePath, std::fstream::in | std::fstream::ate);

        if (!headerFileStream.is_open())
        {
            return {}; // TODO: Exception?
        }

        const auto fileSize = headerFileStream.tellg();
        std::vector<char> fileContents(fileSize);
        headerFileStream.seekg(std::fstream::beg);
        headerFileStream.read(fileContents.data(), fileSize);
        return fileContents;
    }

    std::vector<std::string> split_with_delimiter(std::stringstream& in, char delimiter)
    {
        std::vector<std::string> out;
        std::string s;

        while (std::getline(in, s, delimiter))
        {
            out.push_back(s);
        }

        return out;
    }
}

HeaderFile load_file(const std::string& file_path)
{
    auto file_contents = load_file_contents(file_path);
    HeaderFile file(file_path);

    auto iterator = std::remove_if(file_contents.begin(), file_contents.end(), [](char v){ return !std::isdigit(v) && v != ' '; });
    auto end = file_contents.erase(iterator);

    std::stringstream buffer;
    buffer << std::string(file_contents.begin(), end);

    const auto numberStrings = split_with_delimiter(buffer, ' ');
    std::vector<std::size_t> numbers;

    for (const auto& numberString : numberStrings)
    {
        if (numberString.empty())
        {
            continue;
        }

        const auto val = std::atoi(numberString.c_str());
        if (val <= static_cast<int>(MapTileType::_SIZE))
        {
            numbers.push_back(val);
        }
    }

    for (const auto& room : saturate_rooms(numbers))
    {
        file.add_room(room);
    }

    return file;
}
