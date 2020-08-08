#pragma once

#include <memory>
#include <string>

#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"
#include "viewport/Viewport.hpp"

class DeathOverlay : public GameEntity
{
public:

    explicit DeathOverlay(std::shared_ptr<Viewport>);
    ~DeathOverlay() override;

    void launch();
    void disable_input();
    void enable_input();

    void update(uint32_t delta_time_ms) override;
    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer);
    bool is_scores_requested() const { return _scores_requested; };

private:

    std::string get_available_controls_msg() const;

    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextEntityID game_over = TextBuffer::INVALID_ENTITY;
        TextEntityID controls = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;

    bool _disabled_input = false;
    bool _launched = false;
    bool _scores_requested = false;
};
