#pragma once

#include <limits>
#include <type_traits>

#include "TextureRegion.hpp"

struct OAMEntry
{
    // Off-map initialization.
    int x = std::numeric_limits<int>::min();
    int y = std::numeric_limits<int>::min();

    bool used = false;
    TextureRegion render_tile{};
};

using OAMEntryID = int;
static_assert(std::is_signed<OAMEntryID>::value, "OAMEntryID type takes negative values.");
