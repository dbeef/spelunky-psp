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
    std::string remove_not_allowed_chars(const char*);
}

int main(int arg_counter, char** args)
{
    if (arg_counter != 3)
    {
        std::cout << "Resource compiler saves .asm file defining byte array for given input file contents.\n"
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
        std::cout << "Exception occured: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Finished successfuly." << std::endl;
    return EXIT_SUCCESS;
}

namespace
{
    std::vector<char> load_file(const char* filename)
    {
        std::ifstream input(filename, std::ifstream::binary | std::ifstream::in);
        if (!input.is_open())
        {
            throw std::runtime_error("Failed to open given file for reading.");
        }

        std::vector<char> output;

        input.seekg(0, std::ifstream::end);
        auto file_len = input.tellg();
        input.seekg(0, std::ifstream::beg);

        output.resize(file_len);
        input.read(output.data(), output.size());

        if (!input.good() && input.eof())
        {
            throw std::runtime_error("Failed reading file.");
        }

        return output;
    }

    std::string remove_not_allowed_chars(const char* input)
    {
        std::string out(input);

        for (auto& c : out)
        {
            if (c == '.')
            {
                c = '_';
            }
        }

        return out;
    }

    std::string parse_input(const std::vector<char>& binary_input, const char* input_path)
    {
        std::stringstream out;

        const auto symbol_name = remove_not_allowed_chars(input_path);

        out << "    .section .rodata\n"
            << "    .align 1\n"
            << "    .global " << symbol_name << '\n'
            << "    .hidden " << symbol_name << '\n'
            << symbol_name << ':' << '\n'
            << "    .byte ";

        for (std::size_t index = 0; index < binary_input.size(); index++)
        {
            const auto& byte = binary_input.at(index);
            out << std::to_string(static_cast<signed>(byte));
            if (index != binary_input.size() - 1)
            {
                out << ',';
            }
        }

        out << '\n'
            << "    .align 4\n"
            << "    .global " << symbol_name << "_length" << '\n'
            << "    .hidden " << symbol_name << "_length" << '\n'
            << symbol_name << "_length" << ':' << '\n'
            << "    .4byte "
            << static_cast<std::int32_t>(binary_input.size());

        return out.str();
    }

    void save_file(const char* filename, const std::string& contents)
    {
        std::ofstream out(filename, std::ofstream::out);
        if (!out.is_open())
        {
            throw std::runtime_error("Failed to open given file for writing.");
        }

        out << contents;
    }
}
