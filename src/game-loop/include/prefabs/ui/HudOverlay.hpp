#pragma once

#include "entt/entt.hpp"
#include "components/specialized/HudOverlayComponent.hpp"

namespace prefabs
{
    struct HudOverlay
    {
        static entt::entity create(std::shared_ptr<Viewport>);
    };
}
