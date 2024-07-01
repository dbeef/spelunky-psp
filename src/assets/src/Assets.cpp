#include "assets/Assets.hpp"
#include "logger/log.h"

// FIXME: Only when building with real FS enabled; Move to separate file
#include <filesystem>
#include <optional>
#include <vector>
#include <fstream>
#include <functional>
#include <map>

namespace
{
    const std::filesystem::path SEARCH_PATHS[] =
        {
        "../assets/",
        "./assets/"
    };

    std::optional<std::filesystem::path> detect_assets_path()
    {
        for (const auto& path : SEARCH_PATHS) {
            log_info("Checking for assets presence in: %s", path.c_str());
            if (std::filesystem::exists(path)) {
                return path;
            }
        }
        return {};
    }

    void find_files_recursive(
        const std::filesystem::path& directory_path,
        const std::function<void(const std::filesystem::path& file_path)>& on_file_found)
    {
        for (const auto& entry  : std::filesystem::directory_iterator(directory_path))
        {
            if (entry.is_directory())
            {
                find_files_recursive(entry, on_file_found);
            }
            else if (entry.is_regular_file())
            {
                on_file_found(entry);
            }
        }
    }

    std::optional<Assets::FileContents> load_file(const std::filesystem::path& file_path)
    {
        std::ifstream input(file_path, std::ifstream::binary | std::ifstream::in);
        if (!input.is_open())
        {
            log_error("Failed to open given file for reading: %s", file_path.c_str());
            return {};
        }

        std::vector<char> output;

        input.seekg(0, std::ifstream::end);
        auto file_len = input.tellg();
        input.seekg(0, std::ifstream::beg);

        output.resize(file_len);
        input.read(output.data(), output.size());

        if (!input.good() && input.eof())
        {
            log_error("Failed reading file: %s", file_path.c_str());
            return {};
        }

        return output;
    }

    Assets::FileDatabase generate_file_database(const std::filesystem::path& assets_path)
    {
        Assets::FileDatabase out_db;
        const auto on_file_found = [&out_db, &assets_path](const std::filesystem::path& path) {
            auto contents = load_file(path);
            if (contents) {

                // todo helper method: remove prefix
                std::string no_prefix = path.string();
                auto it = no_prefix.find(assets_path.string());
                assert(it != no_prefix.size());
                no_prefix.erase(it, assets_path.string().size());

                out_db.insert_or_assign(no_prefix, *contents);
            }
        };
        find_files_recursive(assets_path, on_file_found);
        return out_db;
    }
}

REGISTER_SINGLETON_INSTANCE(Assets)

Assets::Assets() = default;

bool Assets::load()
{
    log_info("Entering Assets::load");

    std::optional<std::filesystem::path> assets_path = detect_assets_path();

    if (!assets_path) {
        log_error("Failed to find assets directory!");
        return false;
    }

    log_info("Assets found: %s", assets_path->c_str());
    _database = generate_file_database(*assets_path);

    log_info("Reading database:");
    for (const auto& kv : _database) {
        log_info("%s: %llu bytes", kv.first.c_str(), kv.second);
    }

    log_info("Exiting Assets::load, success.");
    return true;
}

std::pair<const char *, std::size_t> Assets::get(const char *path) {
    auto entry_it = _database.find(path);
    assert(entry_it != _database.end());
    return {entry_it->second.data(), entry_it->second.size()};
}

std::size_t Assets::get_size(const char* path) {
    auto entry_it = _database.find(path);
    assert(entry_it != _database.end());
    return entry_it->second.size();
}