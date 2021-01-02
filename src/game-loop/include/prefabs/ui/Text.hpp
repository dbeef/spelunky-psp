#pragma once

#include "entt/entt.hpp"
#include <string>

namespace prefabs
{
    struct Text
    {
        static entt::entity create(float x, float y, const std::string& contents);
        static entt::entity create();
    };
}
