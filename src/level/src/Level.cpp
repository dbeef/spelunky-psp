#include "Level.hpp"

#include "EntityRegistry.hpp"
#include "generator/Pipeline.hpp"
#include "Stage.hpp"

REGISTER_SINGLETON_INSTANCE(Level)

void Level::generate(LevelType level_type) {
    auto &tile_batch = get_tile_batch();
    auto &registry = EntityRegistry::instance();

    switch (level_type) {
        case LevelType::CAVE: {
            Pipeline::run<LevelType::CAVE>(tile_batch, registry);
            break;
        }
        case LevelType::MAIN_MENU: {
            Pipeline::run<LevelType::MAIN_MENU>(tile_batch, registry);
            break;
        }
        case LevelType::LEVEL_SUMMARY: {
            Pipeline::run<LevelType::LEVEL_SUMMARY>(tile_batch, registry);
            break;
        }
        case LevelType::SCORES: {
            Pipeline::run<LevelType::SCORES>(tile_batch, registry);
            break;
        }
        case LevelType::SANDBOX: {
            Pipeline::run<LevelType::SANDBOX>(tile_batch, registry);
            break;
        }
        default: {
            assert(false);
            break;
        }
    }
}
