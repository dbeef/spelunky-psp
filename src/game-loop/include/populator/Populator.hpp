#pragma once

#include <memory>
#include <vector>
#include <entt/entt.hpp>

class LevelSummaryTracker;

class Populator
{
public:
    void generate_loot(bool robbed);
    void generate_npc(bool& damsel_rescued, bool shopkeeper_robbed);
    void generate_inventory_items(entt::entity main_dude);

    const std::vector<entt::entity>& get_shopkeepers() const { return _shopkeepers; }
    const std::vector<entt::entity>& get_collectibles() const { return _collectibles; }
    const std::vector<entt::entity>& get_npcs() const { return _npcs; }

private:
    std::vector<entt::entity> _shopkeepers;
    std::vector<entt::entity> _collectibles;
    std::vector<entt::entity> _npcs;
};
