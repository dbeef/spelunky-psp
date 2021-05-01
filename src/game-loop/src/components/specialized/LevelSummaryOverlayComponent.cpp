#include "components/specialized/LevelSummaryOverlayComponent.hpp"
#include "components/specialized/LevelSummaryTracker.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/PositionComponent.hpp"

#include "prefabs/collectibles/SingleGoldBar.hpp"
#include "prefabs/collectibles/TripleGoldBar.hpp"
#include "prefabs/collectibles/GoldChunk.hpp"
#include "prefabs/collectibles/GoldNugget.hpp"
#include "prefabs/collectibles/BigGem.hpp"
#include "prefabs/npc/Snake.hpp"
#include "prefabs/npc/Spider.hpp"
#include "prefabs/npc/Skeleton.hpp"
#include "prefabs/npc/Bat.hpp"
#include "prefabs/npc/Caveman.hpp"

#include "EntityRegistry.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

namespace
{
    std::string get_level_completed_msg(uint32_t level_count)
    {
        std::stringstream out;
        out << "LEVEL " << level_count << " COMPLETED!";
        return out.str();
    }

    std::string get_time_msg(uint32_t time_playing_current_level_ms, uint32_t time_playing_total_ms)
    {
        const auto time_playing_current_level_s = static_cast<uint32_t>(time_playing_current_level_ms / 1000);
        const auto time_playing_current_level_minutes = static_cast<uint32_t>(time_playing_current_level_s / 60);

        const auto time_playing_total_s = static_cast<uint32_t>(time_playing_total_ms / 1000);
        const auto time_playing_total_minutes = static_cast<uint32_t>(time_playing_total_s / 60);

        std::stringstream out;
        out << "TIME: " << std::setw(2) << std::right << std::setfill('0') << time_playing_current_level_minutes << ":"
                        << std::setw(2) << std::right << std::setfill('0') << time_playing_current_level_s % 60 << " / "
                        << std::setw(2) << std::right << std::setfill('0') << time_playing_total_minutes << ":"
                        << std::setw(2) << std::right << std::setfill('0') << time_playing_total_s % 60;
        return out.str();
    }

    std::string get_money_msg(uint32_t money_beginning, uint32_t money_end)
    {
        std::stringstream out;
        out << "MONEY: " << "$" << money_beginning << " / " << "$" << money_end;
        return out.str();
    }

    const char* LOOT_MSG = "LOOT: ";
    const char* LOOT_NONE_MSG = "LOOT: NONE";

    const char* KILLS_MSG = "KILLS: ";
    const char* KILLS_NONE_MSG = "KILLS: NONE";
}

LevelSummaryOverlayComponent::LevelSummaryOverlayComponent(const std::shared_ptr<Viewport>& viewport,
                                         const std::shared_ptr<LevelSummaryTracker>& tracker)
    : _viewport(std::move(viewport))
    , _level_summary_tracker(tracker)
{
    auto& registry = EntityRegistry::instance().get_registry();

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        text.set_text(get_level_completed_msg(tracker->get_current_level()));
        text.set_yellow(true);

        position.x_center = _viewport->get_width_world_units() * 0.16f;
        position.y_center = _viewport->get_height_world_units() * 0.135f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);
    }

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        text.set_text(get_time_msg(tracker->get_time_playing_current_level_ms(), tracker->get_time_playing_total_ms()));

        position.x_center = _viewport->get_width_world_units() * 0.16f;
        position.y_center = _viewport->get_height_world_units() * 0.210f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);
    }

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        if (tracker->get_loot_collected_events().empty())
        {
            text.set_text(LOOT_NONE_MSG);
        }
        else
        {
            text.set_text(LOOT_MSG);
        }

        position.x_center = _viewport->get_width_world_units() * 0.16f;
        position.y_center = _viewport->get_height_world_units() * 0.285f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);
    }

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        if (tracker->get_npc_killed_events().empty())
        {
            text.set_text(KILLS_NONE_MSG);
        }
        else
        {
            text.set_text(KILLS_MSG);
        }

        position.x_center = _viewport->get_width_world_units() * 0.16f;
        position.y_center = _viewport->get_height_world_units() * 0.360f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);
    }

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        text.set_text(get_money_msg(tracker->get_dollars_beginning(), tracker->get_dollars_end()));

        position.x_center = _viewport->get_width_world_units() * 0.16f;
        position.y_center = _viewport->get_height_world_units() * 0.435f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);
    }

    _level_summary_tracker->sort_loot_collected_events();
    _level_summary_tracker->sort_npc_killed_events();
}

void LevelSummaryOverlayComponent::update(uint32_t delta_time_ms)
{
    // Make consecutive loot elements appear in an interval:

    auto& registry = EntityRegistry::instance().get_registry();
    _loot_appearing_timer += delta_time_ms;

    const auto& loot_events = _level_summary_tracker->get_loot_collected_events();

    int elements_appearing = std::floor<int>(static_cast<float>(_loot_appearing_timer) / 150.0f);
    for (std::size_t index = _loot_spawned; index < elements_appearing && index < loot_events.size(); index++)
    {
        auto& current_event = loot_events[index];
        float x = 6.25f + (0.25 * index);
        float y = 3.25f;

        switch (current_event)
        {
            // Spawn off-screen loot that will appear on the screen at intervals in the update method:
            case LootCollectedEvent::SINGLE_GOLD_BAR:
            {
                auto entity = prefabs::SingleGoldBar::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                break;
            }
            case LootCollectedEvent::TRIPLE_GOLD_BAR:
            {
                auto entity = prefabs::TripleGoldBar::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                break;
            }
            case LootCollectedEvent::BIG_GEM:
            {
                auto entity = prefabs::BigGem::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                break;
            }
            case LootCollectedEvent::GOLD_CHUNK:
            {
                auto entity = prefabs::GoldChunk::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                break;
            }
            case LootCollectedEvent::GOLD_NUGGET:
            {
                auto entity = prefabs::GoldNugget::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                break;
            }
        }
        _loot_spawned++;
    }

    if (_loot_spawned < loot_events.size())
    {
        return;
    }

    _kills_appearing_timer += delta_time_ms;
    const auto& killed_events = _level_summary_tracker->get_npc_killed_events();

    elements_appearing = std::floor<int>(static_cast<float>(_kills_appearing_timer) / 150.0f);

    for (std::size_t index = _kills_spawned; index < elements_appearing && index < killed_events.size(); index++)
    {
        auto& current_event = killed_events[index];
        float x = 6.75f + (0.25f * index);
        float y = 4.25f;

        switch (current_event)
        {
            case NpcType::SNAKE:
            {
                auto entity = prefabs::Snake::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                registry.remove<ScriptingComponent>(entity);
                break;
            }
            case NpcType::BAT:
            {
                auto entity = prefabs::Bat::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                registry.remove<ScriptingComponent>(entity);
                break;
            }
            case NpcType::SKELETON:
            {
                auto entity = prefabs::Skeleton::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                registry.remove<ScriptingComponent>(entity);
                break;
            }
            case NpcType::SPIDER:
            {
                auto entity = prefabs::Spider::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                registry.remove<ScriptingComponent>(entity);
                break;
            }
            case NpcType::CAVEMAN:
            {
                auto entity = prefabs::Caveman::create(x, y);
                registry.remove<PhysicsComponent>(entity);
                registry.remove<ScriptingComponent>(entity);
                break;
            }
        }
        _kills_spawned++;
    }
}
