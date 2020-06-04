#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "Point2D.hpp"

class StartSign : public GameObject
{
public:

    StartSign(float pos_x_center, float pos_y_center)
            :_quad(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, START_SIGN_QUAD_WIDTH_WORLD_UNITS, START_SIGN_QUAD_HEIGHT_WORLD_UNITS)
            , _center {pos_x_center, pos_y_center}
    {
        _quad.frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::START);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(_center.x, _center.y);
    }

private:

    const float START_SIGN_QUAD_WIDTH_WORLD_UNITS = 3;
    const float START_SIGN_QUAD_HEIGHT_WORLD_UNITS = 2;
    Point2D _center;
    QuadComponent _quad;
};
