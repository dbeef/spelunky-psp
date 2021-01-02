#pragma once

#include "entt/entt.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"

namespace prefabs
{
    struct PauseOverlay
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport, PauseOverlayComponent::Type type);
    };
}
