#pragma once

#include "components/damage/HitpointComponent.hpp"
#include "other/InventoryEvent.hpp"
#include "other/LootCollectedEvent.hpp"
#include "other/NpcType.hpp"
#include "patterns/Observer.hpp"

#include <cstdint>
#include <vector>

// FIXME: This is a leftover from before ECS rework. Make LevelSummarySystem + subsequent components instead.
class LevelSummaryTracker : public Observer<InventoryEvent>,
                            public Observer<LootCollectedEvent>,
                            public Observer<DeathEvent>
{
public:

    LevelSummaryTracker();
    ~LevelSummaryTracker() override;

    void update(uint32_t delta_time_ms);
    void on_notify(const InventoryEvent*) override;
    void on_notify(const LootCollectedEvent*) override;
    void on_notify(const DeathEvent*) override;

    void entered_new_level();
    void reset();

    uint32_t get_current_level() const { return _level_counter; }
    uint32_t get_time_playing_total_ms() const { return _time_playing_total_ms; }
    uint32_t get_time_playing_current_level_ms() const { return _time_playing_current_level_ms; }

    uint32_t get_dollars_beginning() const { return _dollars_beginning; }
    uint32_t get_dollars_end() const { return _dollars_end; }

    void sort_loot_collected_events();
    void sort_npc_killed_events();
    const std::vector<LootCollectedEvent>& get_loot_collected_events() const { return _loot_collected_events; }
    const std::vector<NpcType>& get_npc_killed_events() const { return _npc_killed_events; }

private:
    uint32_t _level_counter = 0;
    uint32_t _time_playing_total_ms = 0;
    uint32_t _time_playing_current_level_ms = 0;

    uint32_t _dollars_beginning = 0;
    uint32_t _dollars_end = 0;

    std::vector<LootCollectedEvent> _loot_collected_events;
    std::vector<NpcType> _npc_killed_events;
};
