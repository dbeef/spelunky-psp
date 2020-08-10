#pragma once

#include <memory>
#include <string>

#include "GameEntity.hpp"
#include "components/TextComponent.hpp"
#include "viewport/Viewport.hpp"

class PauseOverlay : public GameEntity
{
public:

    enum class Type
    {
        MAIN_MENU,
        SCORES,
        PLAYING
    };

    PauseOverlay(std::shared_ptr<Viewport>, Type);
    ~PauseOverlay() override;

    void update(uint32_t delta_time_ms) override;
    void reset();

    void disable_input() { _disabled_input = true; }
    void enable_input() { _disabled_input = false; }

    void unpause() { _paused = false;}

    bool is_paused() const { return _paused; };
    bool is_quit_requested() const { return _quit_requested; }
    bool is_death_requested() const { return _death_requested; }

private:

    std::string get_available_controls_msg() const;

    std::shared_ptr<QuadComponent> _half_opaque_quad;
    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextComponent paused;
        TextComponent controls;
    } _texts;

    bool _paused = false;
    bool _quit_requested = false;
    bool _death_requested = false;
    bool _disabled_input = false;

    const Type _type;
};
