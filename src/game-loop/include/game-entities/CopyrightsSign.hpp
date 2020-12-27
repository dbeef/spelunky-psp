#pragma once

#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameEntity.hpp"
#include "Point2D.hpp"

class CopyrightsSign : public GameEntity
{
public:

    CopyrightsSign(float pos_x_center, float pos_y_center)
        : GameEntity(GameEntity::Type::COPYRIGHTS_SIGN)
        , _center {pos_x_center, pos_y_center}
    {
        _quad_component = std::make_shared<QuadComponent>(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, QUAD_WIDTH_WORLD_UNITS, QUAD_HEIGHT_WORLD_UNITS);
        _quad_component->frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::COPYRIGHTS);
        _quad_component->update(_center.x, _center.y);
    }

    void update(World* world, uint32_t delta_time_ms) override {}

private:

    const float QUAD_WIDTH_WORLD_UNITS = 16.25f;
    const float QUAD_HEIGHT_WORLD_UNITS = 1;
    Point2D _center;
};
