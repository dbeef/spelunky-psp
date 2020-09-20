#include "game-entities/BigGem.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/PhysicsComponentAggregator.hpp"
#include "other/Inventory.hpp"
#include "audio/Audio.hpp"

namespace
{
    const float BIG_GEM_PHYSICAL_WIDTH = 0.5f;
    const float BIG_GEM_PHYSICAL_HEIGHT = 0.5f;

    const float BIG_GEM_QUAD_WIDTH = 0.5f;
    const float BIG_GEM_QUAD_HEIGHT = 0.5f;
}

BigGem::BigGem(float x_center, float y_center)
        : _physics(BIG_GEM_PHYSICAL_WIDTH, BIG_GEM_PHYSICAL_HEIGHT, PhysicsComponentType::BIG_GEM)
        , _quad(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, BIG_GEM_QUAD_WIDTH, BIG_GEM_QUAD_HEIGHT)
{
    // Randomize between diamond/emerald/ruby:
    auto gem_type = static_cast<CollectiblesSpritesheetFrames>(static_cast<int>(CollectiblesSpritesheetFrames::DIAMOND_BIG) + std::rand() % 3);
    _quad.frame_changed<CollectiblesSpritesheetFrames>(gem_type);

    _physics.set_position(x_center, y_center);
    _physics.set_collision_handler([this](PhysicsComponent* other)
    {
        if (is_marked_for_disposal())
        {
            return;
        }

        if (other->get_type() == PhysicsComponentType::MAIN_DUDE)
        {
            Inventory::instance().add_dollars(1000);
            Audio::instance().play(SFXType::COIN);
            mark_for_disposal();
        }
    });
}

void BigGem::update(uint32_t delta_time_ms)
{
    _quad.update(_physics.get_x_position(), _physics.get_y_position());
    _physics.update(delta_time_ms);
}
