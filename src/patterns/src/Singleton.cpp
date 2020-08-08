#include <cassert>

#include "Singleton.hpp"

Singleton* Singleton::_instance = nullptr;

Singleton &Singleton::instance()
{
    assert(_instance);
    return *_instance;
}

void Singleton::init()
{
    assert(!_instance);
    _instance = new Singleton();
}

void Singleton::dispose()
{
    assert(_instance);
    delete _instance;
    _instance = nullptr;
}

