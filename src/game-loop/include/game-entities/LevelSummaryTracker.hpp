#pragma once

#include "game-entities/GameEntity.hpp"
#include "other/InventoryEvent.hpp"
#include "patterns/Observer.hpp"

#include <cstdint>

class LevelSummaryTracker : public GameEntity, public Observer<InventoryEvent>
{
public:

    LevelSummaryTracker();
    ~LevelSummaryTracker() override;

    void update(uint32_t delta_time_ms) override;
    void on_notify(const InventoryEvent*) override;

    void entered_new_level();
    void reset();

    uint32_t get_current_level() const { return _level_counter; }
    uint32_t get_time_playing_total_ms() const { return _time_playing_total_ms; }
    uint32_t get_time_playing_current_level_ms() const { return _time_playing_current_level_ms; }

    uint32_t get_dollars_beginning() const { return _dollars_beginning; }
    uint32_t get_dollars_end() const { return _dollars_end; }

private:
    uint32_t _level_counter = 0;
    uint32_t _time_playing_total_ms = 0;
    uint32_t _time_playing_current_level_ms = 0;

    uint32_t _dollars_beginning = 0;
    uint32_t _dollars_end = 0;
};
