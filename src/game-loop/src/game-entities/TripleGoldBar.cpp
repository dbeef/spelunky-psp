#include "game-entities/TripleGoldBar.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/Inventory.hpp"
#include "audio/Audio.hpp"
#include "other/World.hpp"
#include "main-dude/MainDude.hpp"

namespace
{
    const float TRIPLE_GOLD_BAR_PHYSICAL_WIDTH = 1.0f;
    const float TRIPLE_GOLD_BAR_PHYSICAL_HEIGHT = 1.0f;

    const float TRIPLE_GOLD_BAR_QUAD_WIDTH = 1.0f;
    const float TRIPLE_GOLD_BAR_QUAD_HEIGHT = 1.0f;
}

TripleGoldBar::TripleGoldBar(float x_center, float y_center) : GameEntity(GameEntity::Type::TRIPLE_GOLD_BAR)
{
    _quad_component = std::make_shared<QuadComponent>(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE, TRIPLE_GOLD_BAR_QUAD_WIDTH, TRIPLE_GOLD_BAR_QUAD_HEIGHT);
    _quad_component->frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::TRIPLE_GOLD_BAR);

    _physics_component = std::make_shared<PhysicsComponent>(TRIPLE_GOLD_BAR_PHYSICAL_WIDTH, TRIPLE_GOLD_BAR_PHYSICAL_HEIGHT);
    _physics_component->set_position(x_center, y_center);
}

void TripleGoldBar::update(World* world, uint32_t delta_time_ms) {
    _quad_component->update(_physics_component->get_x_position(), _physics_component->get_y_position());
    _physics_component->update(world, delta_time_ms);

    auto *dude_physics = world->get_main_dude()->get_physics_component();

    if (_physics_component->is_collision(*dude_physics)) {
        // TODO: Inventory subscribing on loot collected events + remove singleton?
        //       First try to implement picking up items, then see
        // TODO: Review & cleanup
        // TODO: Apply Collectible/Prop
        // TODO: What about HUD? A special case of GameEntity?

        Inventory::instance().add_dollars(1000);
        Audio::instance().play(SFXType::COIN);
        notify(LootCollectedEvent::TRIPLE_GOLD_BAR);
        mark_for_disposal();
    }
}
