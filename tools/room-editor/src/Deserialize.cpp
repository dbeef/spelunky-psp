#include "Deserialize.hpp"
#include <fstream>

std::vector<char> load_file(const std::string &filePath)
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
