#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "Point2D.hpp"

class CopyrightsSign : public GameObject
{
public:

    CopyrightsSign(float pos_x_center, float pos_y_center)
            :_quad(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, COPYRIGHTS_QUAD_WIDTH_WORLD_UNITS, COPYRIGHTS_QUAD_HEIGHT_WORLD_UNITS)
            , _center{pos_x_center, pos_y_center}
    {
        _quad.frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::COPYRIGHTS);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(_center.x, _center.y);
    }

private:

    const float COPYRIGHTS_QUAD_WIDTH_WORLD_UNITS = 16.25f;
    const float COPYRIGHTS_QUAD_HEIGHT_WORLD_UNITS = 1;

    Point2D _center;
    QuadComponent _quad;
};
