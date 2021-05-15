#include "components/specialized/LevelSummaryTracker.hpp"
#include "other/Inventory.hpp"

#include <algorithm>

LevelSummaryTracker::LevelSummaryTracker()
{
    Inventory::instance().add_observer(this);
}

LevelSummaryTracker::~LevelSummaryTracker()
{
    Inventory::instance().remove_observer(this);
}

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
    _loot_collected_events.clear();
    _npc_killed_events.clear();
}

void LevelSummaryTracker::reset()
{
    _level_counter = 0;
    _time_playing_total_ms = 0;
    _time_playing_current_level_ms = 0;
    _dollars_beginning = 0;
    _dollars_end = 0;
}

void LevelSummaryTracker::on_notify(const InventoryEvent *event)
{
    if (*event == InventoryEvent::DOLLARS_COUNT_CHANGED)
    {
        _dollars_end = Inventory::instance().get_dollars();
    }
}

void LevelSummaryTracker::on_notify(const LootCollectedEvent *event)
{
    _loot_collected_events.push_back(*event);
}

void LevelSummaryTracker::on_notify(const DeathEvent* event)
{
    _npc_killed_events.push_back(event->npc_type);
}

void LevelSummaryTracker::sort_npc_killed_events()
{
    std::sort(_npc_killed_events.begin(), _npc_killed_events.end(),
              [](const NpcType &e1, const NpcType &e2)
              {
                  return static_cast<int>(e1) < static_cast<int>(e2);
              }
    );
}

void LevelSummaryTracker::sort_loot_collected_events()
{
    std::sort(_loot_collected_events.begin(), _loot_collected_events.end(),
              [](const LootCollectedEvent &e1, const LootCollectedEvent &e2)
              {
                  return static_cast<int>(e1) < static_cast<int>(e2);
              }
    );
}
