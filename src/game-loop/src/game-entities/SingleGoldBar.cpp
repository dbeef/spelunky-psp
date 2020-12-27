#include "game-entities/SingleGoldBar.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/Inventory.hpp"
#include "audio/Audio.hpp"
#include "other/World.hpp"
#include "main-dude/MainDude.hpp"

namespace
{
    const float SINGLE_GOLD_BAR_PHYSICAL_WIDTH = 0.5f;
    const float SINGLE_GOLD_BAR_PHYSICAL_HEIGHT = 0.5f;

    const float SINGLE_GOLD_BAR_QUAD_WIDTH = 0.5f;
    const float SINGLE_GOLD_BAR_QUAD_HEIGHT = 0.5f;
}

SingleGoldBar::SingleGoldBar(float x_center, float y_center) : GameEntity(GameEntity::Type::SINGLE_GOLD_BAR)
{
    _quad_component = std::make_shared<QuadComponent>(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, SINGLE_GOLD_BAR_QUAD_WIDTH, SINGLE_GOLD_BAR_QUAD_HEIGHT);
    _quad_component->frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::SINGLE_GOLD_BAR);

    _physics_component = std::make_shared<PhysicsComponent>(SINGLE_GOLD_BAR_PHYSICAL_WIDTH, SINGLE_GOLD_BAR_PHYSICAL_HEIGHT);
    _physics_component->set_position(x_center, y_center);
}

void SingleGoldBar::update(World* world, uint32_t delta_time_ms)
{
    _quad_component->update(_physics_component->get_x_position(), _physics_component->get_y_position());
    _physics_component->update(world, delta_time_ms);

    auto* dude_physics = world->get_main_dude()->get_physics_component();

    if (_physics_component->is_collision(*dude_physics))
    {
        Inventory::instance().add_dollars(500);
        Audio::instance().play(SFXType::COIN);
        notify(LootCollectedEvent::SINGLE_GOLD_BAR);
        mark_for_disposal();
    }
}
