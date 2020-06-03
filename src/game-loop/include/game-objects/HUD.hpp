#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "HUDSpritesheetFrames.hpp"

class HUD : public GameObject
{
public:

    HUD(float pos_x_center, float pos_y_center)
            :_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, HEART_WIDTH_PIXELS, HEART_HEIGHT_PIXELS)
            , _position {pos_x_center, pos_y_center}
    {
        _quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(*this, delta_time_ms);
    }

private:

    const float HEART_WIDTH_PIXELS = 16;
    const float HEART_HEIGHT_PIXELS = 16;

    struct
    {
        float x_center;
        float y_center;
    } _position;

    friend class QuadComponent;
    QuadComponent _quad;
};
