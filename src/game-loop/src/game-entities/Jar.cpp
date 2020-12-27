#include "game-entities/Jar.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"


namespace
{
    const float JAR_PHYSICAL_WIDTH = 1.0f;
    const float JAR_PHYSICAL_HEIGHT = 1.0f;

    const float JAR_QUAD_WIDTH = 1.0f;
    const float JAR_QUAD_HEIGHT = 1.0f;
}

Jar::Jar(float x_center, float y_center) : GameEntity(GameEntity::Type::JAR)
{
    _quad_component = std::make_shared<QuadComponent>(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, JAR_QUAD_WIDTH, JAR_QUAD_HEIGHT);
    _quad_component->frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::JAR);

    _physics_component = std::make_shared<PhysicsComponent>(JAR_PHYSICAL_WIDTH, JAR_PHYSICAL_HEIGHT);
    _physics_component->set_position(x_center, y_center);
}

void Jar::update(World* world, uint32_t delta_time_ms)
{
    _quad_component->update(_physics_component->get_x_position(), _physics_component->get_y_position());
    _physics_component->update(world, delta_time_ms);
}
