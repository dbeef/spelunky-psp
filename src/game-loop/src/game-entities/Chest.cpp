#include "game-entities/Chest.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"

namespace
{
    const float CHEST_PHYSICAL_WIDTH = 1.0f;
    const float CHEST_PHYSICAL_HEIGHT = 1.0f;

    const float CHEST_QUAD_WIDTH = 1.0f;
    const float CHEST_QUAD_HEIGHT = 1.0f;
}

Chest::Chest(float x_center, float y_center) : GameEntity(GameEntity::Type::CHEST)
{
    _quad_component = std::make_shared<QuadComponent>(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, CHEST_QUAD_WIDTH, CHEST_QUAD_HEIGHT);
    _quad_component->frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::CHEST);

    _physics_component = std::make_shared<PhysicsComponent>(CHEST_PHYSICAL_WIDTH, CHEST_PHYSICAL_HEIGHT);
    _physics_component->set_position(x_center, y_center);
}

void Chest::update(World* world, uint32_t delta_time_ms)
{
    _quad_component->update(_physics_component->get_x_position(), _physics_component->get_y_position());
    _physics_component->update(world, delta_time_ms);
}
