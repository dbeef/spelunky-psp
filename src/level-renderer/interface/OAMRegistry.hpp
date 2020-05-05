#pragma once

#include <vector>
#include "OAMEntry.hpp"

class OAMRegistry
{
public:

    static OAMRegistry& instance();
    static void init();
    static void dispose();

    OAMRegistry();
    void clear_entries();
    OAMEntryID get_OAM_entry();
    void free_OAM_entry(OAMEntryID id);
    const bool is_invalid(OAMEntryID id) { return id == INVALID_ENTRY; }

private:

    static OAMRegistry* _instance;
    static const OAMEntryID INVALID_ENTRY = -1;
    std::vector<OAMEntry> _entries;
};
