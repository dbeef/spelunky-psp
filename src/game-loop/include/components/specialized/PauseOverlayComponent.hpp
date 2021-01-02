#pragma once

#include <memory>
#include <string>

#include "entt/entt.hpp"


#include "components/generic/TextComponent.hpp"
#include "viewport/Viewport.hpp"

class PauseOverlayComponent
{
public:

    enum class Type
    {
        MAIN_MENU,
        SCORES,
        PLAYING
    };

    PauseOverlayComponent(std::shared_ptr<Viewport>, Type);
    PauseOverlayComponent() = default;
    PauseOverlayComponent(const PauseOverlayComponent&);

    void update(entt::registry& registry);

    void disable_input() { _disabled_input = true; }
    void enable_input() { _disabled_input = false; }

    bool is_paused() const { return _paused; };
    bool is_quit_requested() const { return _quit_requested; }
    bool is_death_requested() const { return _death_requested; }

    void unpause() { _paused = false; }
    void hide(entt::registry&);

private:

    std::string get_available_controls_msg() const;
    std::shared_ptr<Viewport> _viewport;
    std::vector<entt::entity> _children;

    bool _paused = false;
    bool _quit_requested = false;
    bool _death_requested = false;
    bool _disabled_input = false;

    Type _type;
};
