#pragma once

#include <memory>
#include <vector>

#include <entt/entt.hpp>
#include <components/specialized/HudOverlayComponent.hpp>

class GameEntity;
class LevelSummaryTracker;

namespace populator
{
    void generate_loot(std::shared_ptr<LevelSummaryTracker>&);
    void generate_npc(std::shared_ptr<LevelSummaryTracker>&, bool& damsel_rescued, bool& shopkeeper_robbed);
    void generate_inventory_items(entt::entity main_dude);
}
