#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "Point2D.hpp"

class MainLogo : public GameObject
{
public:

    MainLogo(float pos_x_center, float pos_y_center)
            :_quad(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, LOGO_QUAD_WIDTH_WORLD_UNITS, LOGO_QUAD_HEIGHT_WORLD_UNITS)
            , _center {pos_x_center, pos_y_center}
    {
        _quad.frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::MAIN_LOGO);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(_center.x, _center.y);
    }

private:

    const float LOGO_QUAD_WIDTH_WORLD_UNITS = 12;
    const float LOGO_QUAD_HEIGHT_WORLD_UNITS = 5;
    Point2D _center;
    QuadComponent _quad;
};
