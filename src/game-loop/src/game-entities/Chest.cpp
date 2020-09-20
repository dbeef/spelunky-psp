#include "game-entities/Chest.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/PhysicsComponentAggregator.hpp"

namespace
{
    const float CHEST_PHYSICAL_WIDTH = 1.0f;
    const float CHEST_PHYSICAL_HEIGHT = 1.0f;

    const float CHEST_QUAD_WIDTH = 1.0f;
    const float CHEST_QUAD_HEIGHT = 1.0f;
}

Chest::Chest(float x_center, float y_center)
        : _physics(CHEST_PHYSICAL_WIDTH, CHEST_PHYSICAL_HEIGHT, PhysicsComponentType::CHEST)
        , _quad(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, CHEST_QUAD_WIDTH, CHEST_QUAD_HEIGHT)
{
    _quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::CHEST);
    _physics.set_position(x_center, y_center);
}

void Chest::update(uint32_t delta_time_ms)
{
    _quad.update(_physics.get_x_position(), _physics.get_y_position());
    _physics.update(delta_time_ms);
}
