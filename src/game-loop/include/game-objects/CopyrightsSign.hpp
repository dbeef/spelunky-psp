#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"

class CopyrightsSign : public GameObject
{
public:

    CopyrightsSign(float pos_x_center, float pos_y_center)
            :_quad(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, COPYRIGHTS_QUAD_WIDTH, COPYRIGHTS_QUAD_HEIGHT)
            , _position {pos_x_center, pos_y_center}
    {
        _quad.frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::COPYRIGHTS);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(_position.x_center, _position.y_center);
    }

private:

    const float COPYRIGHTS_QUAD_WIDTH = 16.25f;
    const float COPYRIGHTS_QUAD_HEIGHT = 1;

    struct
    {
        float x_center;
        float y_center;
    } _position;

    friend class QuadComponent;
    QuadComponent _quad;
};
