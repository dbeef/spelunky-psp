#pragma once

#include "InputComponent.hpp"
#include "entt/entt.hpp"

struct OpenableComponent
{
    bool opened = false;
    entt::entity who_opened = entt::null;
};
