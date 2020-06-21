#pragma once

#include <memory>

#include "GameObject.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"
#include "viewport/Viewport.hpp"

class HUD : public GameObject
{
public:

    explicit HUD(std::shared_ptr<Viewport>);
    ~HUD() override;

    void update(uint32_t delta_time_ms) override;
    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer);
    void set_hearts_count(uint32_t hearts);
    void set_ropes_count(uint32_t ropes);
    void set_bombs_count(uint32_t bombs);
    void set_dollars_count(uint32_t dollars);

private:

    // TODO: Scale depending on screen resolution.
    const float ICON_WIDTH_PIXELS = 16;
    const float ICON_HEIGHT_PIXELS = 16;
    float icons_offset_pixels = 0.0f;

    Point2D _heart_center;
    Point2D _dollar_center;
    Point2D _ropes_center;
    Point2D _bombs_center;
    Point2D _hold_item_center;

    QuadComponent _heart_quad;
    QuadComponent _dollar_quad;
    QuadComponent _ropes_quad;
    QuadComponent _bombs_quad;
    QuadComponent _hold_item_quad;

    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<Viewport> _viewport;
    struct
    {
        TextEntityID hearts = TextBuffer::INVALID_ENTITY;
        TextEntityID ropes = TextBuffer::INVALID_ENTITY;
        TextEntityID bombs = TextBuffer::INVALID_ENTITY;
        TextEntityID dollars = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;
};
