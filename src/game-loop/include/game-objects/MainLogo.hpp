#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"

class MainLogo : public GameObject
{
public:

    MainLogo(float pos_x_center, float pos_y_center)
            :_quad(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, LOGO_QUAD_WIDTH, LOGO_QUAD_HEIGHT)
            , _position {pos_x_center, pos_y_center}
    {
        _quad.frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::MAIN_LOGO);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(_position.x_center, _position.y_center);
    }

private:

    const float LOGO_QUAD_WIDTH = 12;
    const float LOGO_QUAD_HEIGHT = 5;

    struct
    {
        float x_center;
        float y_center;
    } _position;

    friend class QuadComponent;
    QuadComponent _quad;
};
