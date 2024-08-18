#include "assets/Assets.hpp"
#include "VirtualFileIndex.hpp"

#include <logger/log.h>

#include <map>
#include <cstring>
#include <string>

struct Contents {
    Contents(const char* data, std::size_t size) : data(data), size(size) {}
    const char* data;
    std::size_t size;
};
using Path = std::string;
using PathToContentsMapping = std::map<Path, Contents>;

struct Assets::ImplementationDefined {
    PathToContentsMapping files;
};

Assets::~Assets() {
    // For pimpl
}

Assets::Assets() : _impl(std::make_unique<ImplementationDefined>()) {
}

bool Assets::load() {
    for (const auto& virtual_file : get_resource_compiled_files()) {
        log_info("Bundled file: %s", virtual_file.path);
        _impl->files.insert_or_assign(std::string(virtual_file.path), Contents{virtual_file.data, virtual_file.size});
    }

    return true;
}

std::pair<const char *, std::size_t> Assets::get(const char *path) const {
    const auto &files = _impl->files;
    auto entry_it = files.find(path);
    assert(entry_it != files.end());
    return {entry_it->second.data, entry_it->second.size};
}

std::size_t Assets::get_size(const char *path) const {
    const auto &files = _impl->files;
    auto entry_it = files.find(path);
    assert(entry_it != files.end());
    return entry_it->second.size;
}

const char *Assets::get_ptr(const char *path) const {
    const auto &files = _impl->files;
    auto entry_it = files.find(path);
    assert(entry_it != files.end());
    return entry_it->second.data;
}