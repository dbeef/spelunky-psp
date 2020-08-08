#pragma once

#include <memory>
#include <string>

#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "TextBuffer.hpp"
#include "viewport/Viewport.hpp"

class LevelSummaryOverlay : public GameEntity
{
public:

    explicit LevelSummaryOverlay(std::shared_ptr<Viewport>);
    ~LevelSummaryOverlay() override;

    void launch();
    void update(uint32_t delta_time_ms) override;
    void set_text_buffer(const std::shared_ptr<TextBuffer>& text_buffer);

private:

    std::shared_ptr<TextBuffer> _text_buffer;
    std::shared_ptr<Viewport> _viewport;

    struct
    {
        TextEntityID level_completed = TextBuffer::INVALID_ENTITY;
    } _text_entity_ids;

    bool _launched = false;
};
