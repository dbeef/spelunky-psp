#pragma once

#include <memory>
#include <string>

#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"
#include "viewport/Viewport.hpp"

class ScoresOverlay : public GameEntity
{
public:

    explicit ScoresOverlay(std::shared_ptr<Viewport>);
    ~ScoresOverlay() override;

    void launch();
    void update(uint32_t delta_time_ms) override;
    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer);

private:

    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextEntityID top_defilers = TextBuffer::INVALID_ENTITY;
        TextEntityID money = TextBuffer::INVALID_ENTITY;
        TextEntityID kills = TextBuffer::INVALID_ENTITY;
        TextEntityID saves = TextBuffer::INVALID_ENTITY;
        TextEntityID statistics = TextBuffer::INVALID_ENTITY;
        TextEntityID plays = TextBuffer::INVALID_ENTITY;
        TextEntityID deaths = TextBuffer::INVALID_ENTITY;
        TextEntityID wins = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;

    bool _launched = false;
};
