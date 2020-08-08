#include <cstring>
#include "game-entities/LevelSummaryOverlay.hpp"

namespace
{
    const char* LEVEL_COMPLETED_MSG = "LEVEL COMPLETED!";
}

LevelSummaryOverlay::LevelSummaryOverlay(std::shared_ptr<Viewport> viewport) : _viewport(std::move(viewport))
{
}

LevelSummaryOverlay::~LevelSummaryOverlay()
{
    _text_buffer->remove_text(_text_entity_ids.level_completed);
}

void LevelSummaryOverlay::launch()
{
    if (_launched)
    {
        return;
    }

    _text_entity_ids.level_completed = _text_buffer->create_text();

    {
        const Point2D pos = {_viewport->get_width_world_units() * 0.16f, _viewport->get_height_world_units() * 0.175f};
        _text_buffer->update_text(_text_entity_ids.level_completed, pos, LEVEL_COMPLETED_MSG, std::strlen(LEVEL_COMPLETED_MSG), true);
    }

    _launched = true;
}

void LevelSummaryOverlay::update(uint32_t delta_time_ms)
{
    // Nothing to update.
}

void LevelSummaryOverlay::set_text_buffer(const std::shared_ptr<TextBuffer> &text_buffer)
{
    assert(text_buffer != nullptr);
    _text_buffer = text_buffer;
}