#pragma once

#include <memory>
#include <string>

#include "GameObject.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"
#include "viewport/Viewport.hpp"

class DeathOverlay : public GameObject
{
public:

    explicit DeathOverlay(const std::shared_ptr<Viewport>&);
    ~DeathOverlay() override;

    void disable();
    void enable();

    void launch();
    void update(uint32_t delta_time_ms) override;
    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer);

    bool is_launched() const { return _launched; }
    bool is_main_menu_requested() const { return _main_menu_requested; };

private:

    std::string get_available_controls_msg() const;

    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextEntityID game_over = TextBuffer::INVALID_ENTITY;
        TextEntityID controls = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;

    bool _disabled = false;
    bool _launched = false;
    bool _main_menu_requested = false;
};
