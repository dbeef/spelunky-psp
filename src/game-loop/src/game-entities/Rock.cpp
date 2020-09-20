#include "game-entities/Rock.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/PhysicsComponentAggregator.hpp"

namespace
{
    const float ROCK_PHYSICAL_WIDTH = 0.5f;
    const float ROCK_PHYSICAL_HEIGHT = 0.5f;

    const float ROCK_QUAD_WIDTH = 0.5f;
    const float ROCK_QUAD_HEIGHT = 0.5f;
}

Rock::Rock(float x_center, float y_center)
        : _physics(ROCK_PHYSICAL_WIDTH, ROCK_PHYSICAL_HEIGHT, PhysicsComponentType::ROCK)
        , _quad(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, ROCK_QUAD_WIDTH, ROCK_QUAD_HEIGHT)
{
    _quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::ROCK);
    _physics.set_position(x_center, y_center);
}

void Rock::update(uint32_t delta_time_ms)
{
    _quad.update(_physics.get_x_position(), _physics.get_y_position());
    _physics.update(delta_time_ms);
}
