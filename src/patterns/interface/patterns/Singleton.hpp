#pragma once

#include <cassert>

#define REGISTER_SINGLETON_INSTANCE(Type) template<> Type* Singleton<Type>::_instance = nullptr;
#define DELETE_COPY_MOVE_CONSTRUCTORS(Type)     Type(Type&) = delete; \
                                                Type(Type&&) = delete; \
                                                Type(const Type&) = delete; \
                                                Type(const Type&&) = delete;
#define FRIEND_SINGLETON(Type) friend class Singleton<Type>;


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
