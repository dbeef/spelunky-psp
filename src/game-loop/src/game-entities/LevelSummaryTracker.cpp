#include "game-entities/LevelSummaryTracker.hpp"

void LevelSummaryTracker::update(uint32_t delta_time_ms)
{
    _time_playing_total_ms += delta_time_ms;
    _time_playing_current_level_ms += delta_time_ms;
}

void LevelSummaryTracker::entered_new_level()
{
    _level_counter += 1;
    _time_playing_current_level_ms = 0;
}

void LevelSummaryTracker::reset()
{
    _level_counter = 0;
    _time_playing_total_ms = 0;
    _time_playing_current_level_ms = 0;
}
