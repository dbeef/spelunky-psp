#pragma once

#include "Input.hpp"
#include <vector>

struct InputComponent
{
    std::vector<InputEvent> allowed_events;
};
