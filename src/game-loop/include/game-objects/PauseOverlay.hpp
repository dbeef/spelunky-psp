#pragma once

#include <memory>
#include <string>

#include "GameObject.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"
#include "viewport/Viewport.hpp"

class PauseOverlay : public GameObject
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
    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer);

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
    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextEntityID paused = TextBuffer::INVALID_ENTITY;
        TextEntityID controls = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;

    bool _paused = false;
    bool _quit_requested = false;
    bool _death_requested = false;
    bool _disabled_input = false;

    const Type _type;
};
