#pragma once

#include <memory>
#include <vector>

class GameEntity;

namespace populator
{
    std::vector<std::shared_ptr<GameEntity>> generate_loot();
}
