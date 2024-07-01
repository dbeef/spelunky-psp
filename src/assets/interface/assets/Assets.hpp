#pragma once

#include "patterns/Singleton.hpp"

#include <utility>
#include <filesystem>
#include <vector>
#include <map>

class Assets : public Singleton<Assets> {
public:
    using FileContents = std::vector<char>;
    using FileDatabase = std::map<std::filesystem::path, FileContents>;

    bool load();

    std::pair<const char *, std::size_t> get(const char *path);
    std::size_t get_size(const char *path);

    template<class T = const char*>
    T get_ptr(const char* path) {
        auto entry_it = _database.find(path);
        assert(entry_it != _database.end());
        return reinterpret_cast<T>(entry_it->second.data());
    }

    DELETE_COPY_MOVE_CONSTRUCTORS(Assets)

    FRIEND_SINGLETON(Assets)

private:
    FileDatabase _database;

    Assets();
};
