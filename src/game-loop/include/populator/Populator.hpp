#pragma once

#include <memory>
#include <vector>

class GameEntity;
class World;
class LevelSummaryTracker;

namespace populator
{
    std::vector<std::shared_ptr<GameEntity>> generate_loot(std::shared_ptr<LevelSummaryTracker>&, World* world);
}
