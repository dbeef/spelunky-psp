#pragma once

#include "entt/entt.hpp"
#include "components/specialized/DeathOverlayComponent.hpp"

namespace prefabs
{
    struct DeathOverlay
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport);
    };
}
