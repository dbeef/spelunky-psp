#pragma once

#include "Input.hpp"
#include <vector>

struct InputComponent
{
    std::vector<InputEvent> allowed_events;
    bool dont_feed = false; // For passing fake input? FIXME
};
