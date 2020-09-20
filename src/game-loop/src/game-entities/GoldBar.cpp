#include "game-entities/GoldBar.hpp"
#include "Renderer.hpp"
#include "spritesheet-frames/CollectiblesSpritesheetFrames.hpp"
#include "other/PhysicsComponentAggregator.hpp"
#include "other/Inventory.hpp"

namespace
{
    const float GOLD_BAR_PHYSICAL_WIDTH = 0.5f;
    const float GOLD_BAR_PHYSICAL_HEIGHT = 0.5f;

    const float GOLD_BAR_QUAD_WIDTH = 0.5f;
    const float GOLD_BAR_QUAD_HEIGHT = 0.5f;
}

GoldBar::GoldBar(float x_center, float y_center)
        : _physics(GOLD_BAR_PHYSICAL_WIDTH, GOLD_BAR_PHYSICAL_HEIGHT, PhysicsComponentType::SINGLE_GOLD_BAR)
        , _quad(TextureType::COLLECTIBLES, Renderer::EntityType::MODEL_VIEW_SPACE,  GOLD_BAR_QUAD_WIDTH, GOLD_BAR_QUAD_HEIGHT)
{
    _quad.frame_changed<CollectiblesSpritesheetFrames>(CollectiblesSpritesheetFrames::GOLD_BAR);

    _physics.set_position(x_center, y_center);
    _physics.set_collision_handler([this](PhysicsComponent* other)
    {
        if (is_marked_for_disposal())
        {
            return;
        }

        if (other->get_type() == PhysicsComponentType::MAIN_DUDE)
        {
            Inventory::instance().add_dollars(500);
            mark_for_disposal();
        }
    });
}

void GoldBar::update(uint32_t delta_time_ms)
{
    _quad.update(_physics.get_x_position(), _physics.get_y_position());
    _physics.update(delta_time_ms);
}
