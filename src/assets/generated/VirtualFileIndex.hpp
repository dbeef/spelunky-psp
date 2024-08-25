#pragma once

#include <vector>

struct VirtualFile {
    const char* path;
    const char* data;
    std::size_t size;
};

std::vector<VirtualFile> get_resource_compiled_files();
