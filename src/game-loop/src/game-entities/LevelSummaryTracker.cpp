#include "game-entities/LevelSummaryTracker.hpp"
#include "other/Inventory.hpp"

void LevelSummaryTracker::update(uint32_t delta_time_ms)
{
    _time_playing_total_ms += delta_time_ms;
    _time_playing_current_level_ms += delta_time_ms;
}

void LevelSummaryTracker::entered_new_level()
{
    _level_counter += 1;
    _time_playing_current_level_ms = 0;
    _dollars_beginning = _dollars_end;
}

void LevelSummaryTracker::reset()
{
    _level_counter = 0;
    _time_playing_total_ms = 0;
    _time_playing_current_level_ms = 0;
    _dollars_beginning = 0;
    _dollars_end = 0;
}

void LevelSummaryTracker::on_notify(const InventoryEvent* event)
{
    if (*event == InventoryEvent::DOLLARS_COUNT_CHANGED)
    {
        _dollars_end = Inventory::instance().get_dollars();
    }
}

LevelSummaryTracker::LevelSummaryTracker()
{
    Inventory::instance().add_observer(this);
}

LevelSummaryTracker::~LevelSummaryTracker()
{
    Inventory::instance().remove_observer(this);
}
