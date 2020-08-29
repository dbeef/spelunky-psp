#pragma once

#include "game-entities/GameEntity.hpp"
#include <cstdint>

class LevelStatistics : public GameEntity
{
public:
    void update(uint32_t delta_time_ms) override;

    void entered_new_level();
    void reset();

    uint32_t get_current_level() const { return _level_counter; }
    uint32_t get_time_playing_total_ms() const { return _time_playing_total_ms; }
    uint32_t get_time_playing_current_level_ms() const { return _time_playing_current_level_ms; }

private:
    uint32_t _level_counter = 0;
    uint32_t _time_playing_total_ms = 0;
    uint32_t _time_playing_current_level_ms = 0;
};
