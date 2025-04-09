#pragma once

#include "InputComponent.hpp"

struct ActivableComponent
{
    std::vector<InputEvent> activate_combination;
    bool activated = false;
};
