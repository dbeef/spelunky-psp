#pragma once

#include "patterns/Singleton.hpp"

#include <utility>
#include <memory>

class Assets : public Singleton<Assets> {
public:

    ~Assets();

    struct ImplementationDefined;

    bool load();

    std::pair<const char *, std::size_t> get(const char *path) const;

    std::size_t get_size(const char *path) const;

    const char *get_ptr(const char *path) const;

    template<class T>
    T get_ptr(const char *path) const {
        return reinterpret_cast<T>(const_cast<char*>(get_ptr(path)));
    }

    DELETE_COPY_MOVE_CONSTRUCTORS(Assets)

    FRIEND_SINGLETON(Assets)

private:
    std::unique_ptr<ImplementationDefined> _impl;

    Assets();
};
