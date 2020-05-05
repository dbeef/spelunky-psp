#include "OAMRegistry.hpp"

#include <cassert>
#include <algorithm>

namespace
{
    const std::size_t OAM_REGISTRY_INITIAL_SIZE = 256;
}

OAMRegistry* OAMRegistry::_instance = nullptr;

OAMRegistry &OAMRegistry::instance()
{
    assert(_instance);
    return *_instance;
}

void OAMRegistry::init()
{
    assert(!_instance);
    _instance = new OAMRegistry();
}

void OAMRegistry::dispose()
{
    assert(_instance);
    delete _instance;
    _instance = nullptr;
}

int OAMRegistry::get_OAM_entry()
{
    for (std::size_t index = 0; index < _entries.size(); index++)
    {
        if (!_entries[index].used)
        {
            _entries[index].used = true;
            return index;
        }
    }
    return INVALID_ENTRY;
}

void OAMRegistry::free_OAM_entry(int OAM_entry_id)
{
    assert(_entries.size() > OAM_entry_id);
    _entries[OAM_entry_id] = OAMEntry();
}

OAMRegistry::OAMRegistry() : _entries(OAM_REGISTRY_INITIAL_SIZE, OAMEntry())
{
}

void OAMRegistry::clear_entries()
{
    std::for_each(_entries.begin(), _entries.end(), [](OAMEntry &entry)
    { entry = {}; });
}
