#include "game-entities/Jar.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/PhysicsComponentAggregator.hpp"

namespace
{
    const float JAR_PHYSICAL_WIDTH = 1.0f;
    const float JAR_PHYSICAL_HEIGHT = 1.0f;

    const float JAR_QUAD_WIDTH = 1.0f;
    const float JAR_QUAD_HEIGHT = 1.0f;
}

Jar::Jar(float x_center, float y_center)
        : _physics(JAR_PHYSICAL_WIDTH, JAR_PHYSICAL_HEIGHT, PhysicsComponentType::JAR)
        , _quad(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, JAR_QUAD_WIDTH, JAR_QUAD_HEIGHT)
{
    _quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::JAR);
    _physics.set_position(x_center, y_center);
}

void Jar::update(uint32_t delta_time_ms)
{
    _quad.update(_physics.get_x_position(), _physics.get_y_position());
    _physics.update(delta_time_ms);
}
