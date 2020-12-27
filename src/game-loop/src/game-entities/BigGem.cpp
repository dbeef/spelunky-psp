#include "game-entities/BigGem.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

#include "other/Inventory.hpp"
#include "audio/Audio.hpp"

#include <cstdlib>
#include "main-dude/MainDude.hpp"
#include "other/World.hpp"

namespace
{
    const float BIG_GEM_PHYSICAL_WIDTH = 0.5f;
    const float BIG_GEM_PHYSICAL_HEIGHT = 0.5f;

    const float BIG_GEM_QUAD_WIDTH = 0.5f;
    const float BIG_GEM_QUAD_HEIGHT = 0.5f;
}

BigGem::BigGem(float x_center, float y_center) : GameEntity(GameEntity::Type::BIG_GEM)
{
    _physics_component = std::make_shared<PhysicsComponent>(BIG_GEM_PHYSICAL_WIDTH, BIG_GEM_PHYSICAL_HEIGHT);
    _quad_component = std::make_shared<QuadComponent>(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, BIG_GEM_QUAD_WIDTH, BIG_GEM_QUAD_HEIGHT);

    // Randomize between diamond/emerald/ruby (blue/green/red):
    _gem_type = static_cast<CollectiblesSpritesheetFrames>(static_cast<int>(CollectiblesSpritesheetFrames::DIAMOND_BIG) + std::rand() % 3);
    _quad_component->frame_changed<CollectiblesSpritesheetFrames>(_gem_type);
    _physics_component->set_position(x_center, y_center);
}

void BigGem::update(World* world, uint32_t delta_time_ms)
{
    _quad_component->update(_physics_component->get_x_position(), _physics_component->get_y_position());
    _physics_component->update(world, delta_time_ms);

    auto *dude_physics = world->get_main_dude()->get_physics_component();

    if (_physics_component->is_collision(*dude_physics)) {
        Inventory::instance().add_dollars(1000);
        Audio::instance().play(SFXType::COIN);

        switch (_gem_type)
        {
            case CollectiblesSpritesheetFrames::DIAMOND_BIG: notify(LootCollectedEvent::GEM_DIAMOND); break;
            case CollectiblesSpritesheetFrames::EMERALD_BIG: notify(LootCollectedEvent::GEM_EMERALD); break;
            case CollectiblesSpritesheetFrames::RUBY_BIG: notify(LootCollectedEvent::GEM_RUBY); break;
            default: break;
        }

        mark_for_disposal();
    }
}
