#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

namespace
{
    std::vector<char> load_file(const char* filename);
    void save_file(const char* filename, const std::string& contents);
    std::string parse_input(const std::vector<char>& binary_input, const char* input_path);
}

int main(int arg_counter, char** args)
{
    if (arg_counter != 3)
    {
        std::cout << "Resource compiler saves C header file defining char array for given input file contents."
                  << "Usage: <path to input file> <path to output file>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* input_path = args[1];
    const char* output_path = args[2];

    try
    {
        const auto input = load_file(input_path);
        const std::string header = parse_input(input, input_path);
        save_file(output_path, header);
    }
    catch (const std::exception& e)
    {
        std::cout << "Failure: " << e.what() << ", for file: " << input_path << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Success: " << input_path << std::endl;
    return EXIT_SUCCESS;
}

namespace
{
    std::vector<char> load_file(const char* filename)
    {
        std::ifstream input(filename, std::ifstream::binary | std::ifstream::in);
        if (!input.is_open())
        {
            throw std::runtime_error("Failed to open given file for reading");
        }

        std::vector<char> output;

        input.seekg(0, std::ifstream::end);
        auto file_len = input.tellg();
        input.seekg(0, std::ifstream::beg);

        output.resize(file_len);
        input.read(output.data(), output.size());

        if (!input.good() && input.eof())
        {
            throw std::runtime_error("Failed reading file");
        }

        return output;
    }

    std::string parse_input(const std::vector<char>& binary_input, const char* input_path)
    {
        std::stringstream out;

        const auto now = std::chrono::system_clock::now();
        const auto current_time = std::chrono::system_clock::to_time_t(now);

        out << "// Generated from: " << input_path << ", at: " << std::ctime(&current_time) << "\n"
            << "const std::size_t size = " << binary_input.size() << ";\n"
            << "const char* path = \"" << input_path << "\";\n"
            << "static const char data[] = \n{\n    ";

        std::size_t newline_counter = 0;
        for (const char &byte : binary_input)
        {
            out << std::to_string(static_cast<signed>(byte)) << ",";
            newline_counter++;
            if (newline_counter > 16)
            {
                out << "\n    ";
                newline_counter = 0;
            }
        }

        out << "\n};\n";
        return out.str();
    }

    void save_file(const char* filename, const std::string& contents)
    {
        std::ofstream out(filename, std::ofstream::out);
        if (!out.is_open())
        {
            throw std::runtime_error("Failed to open given file for writing");
        }

        out << contents;
    }
}
