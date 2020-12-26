#pragma once

#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameEntity.hpp"
#include "Point2D.hpp"

class ResetSign : public GameEntity
{
public:

    ResetSign(float pos_x_center, float pos_y_center)
            : GameEntity(GameEntity::Type::RESET_SIGN)
             , _center{pos_x_center, pos_y_center}
    {
        _quad_component = std::make_shared<QuadComponent>(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, QUAD_WIDTH_WORLD_UNITS, QUAD_HEIGHT_WORLD_UNITS);
        _quad_component->frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::RESET);
        _quad_component->update(_center.x, _center.y);
    }

    void update(uint32_t delta_time_ms) override {}

private:
    const float QUAD_WIDTH_WORLD_UNITS = 3;
    const float QUAD_HEIGHT_WORLD_UNITS = 1;
    Point2D _center;
};
