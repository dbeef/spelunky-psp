#pragma once

#include <vector>

#include "LevelType.hpp"
#include "populator/Spawner.hpp"

namespace Spawners {
    std::vector<Spawner> get_npc_spawners(LevelType level_type);
    std::vector<Spawner> get_loot_spawners(LevelType level_type);
}
