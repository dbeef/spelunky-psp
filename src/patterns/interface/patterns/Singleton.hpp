#pragma once

#include <cassert>

#define REGISTER_SINGLETON(Type) template<> Type* Singleton<Type>::_instance = nullptr;

template<class T>
class Singleton
{
public:

    static T& instance()
    {
        assert(_instance);
        return *_instance;
    }

    static void init()
    {
        assert(!_instance);
        _instance = new T();
    }

    static void dispose()
    {
        assert(_instance);
        delete _instance;
        _instance = nullptr;
    }

private:
    static T* _instance;
};
