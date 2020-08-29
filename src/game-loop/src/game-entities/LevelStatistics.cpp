#include "game-entities/LevelStatistics.hpp"

void LevelStatistics::update(uint32_t delta_time_ms)
{
    _time_playing_total_ms += delta_time_ms;
    _time_playing_current_level_ms += delta_time_ms;
}

void LevelStatistics::entered_new_level()
{
    _level_counter += 1;
    _time_playing_current_level_ms = 0;
}

void LevelStatistics::reset()
{
    _level_counter = 0;
    _time_playing_total_ms = 0;
    _time_playing_current_level_ms = 0;
}
