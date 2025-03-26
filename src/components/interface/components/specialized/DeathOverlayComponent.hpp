#pragma once

#include <memory>
#include <string>

#include <entt/entt.hpp>
#include "viewport/Viewport.hpp"

#include "components/generic/TextComponent.hpp"

class DeathOverlayComponent
{
public:

    explicit DeathOverlayComponent(std::shared_ptr<Viewport>);

    void launch();
    void disable_input();
    void enable_input();

    void update();
    bool is_scores_requested() const { return _scores_requested; };

private:

    std::string get_available_controls_msg() const;

    std::vector<entt::entity> _children;
    std::shared_ptr<Viewport> _viewport;

    bool _disabled_input = false;
    bool _scores_requested = false;
};
