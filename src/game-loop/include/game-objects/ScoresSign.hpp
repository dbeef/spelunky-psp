#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"

class ScoresSign : public GameObject
{
public:

    ScoresSign(float pos_x_center, float pos_y_center)
            :_quad(TextureType::MAIN_MENU, Renderer::EntityType::MODEL_VIEW_SPACE, SCORES_SIGN_QUAD_WIDTH, SCORES_SIGN_QUAD_HEIGHT)
            , _position {pos_x_center, pos_y_center}
    {
        _quad.frame_changed<MainMenuSpritesheetFrames>(MainMenuSpritesheetFrames::SCORES);
    }

    void update(uint32_t delta_time_ms) override
    {
        _quad.update(*this, delta_time_ms);
    }

private:

    const float SCORES_SIGN_QUAD_WIDTH = 3;
    const float SCORES_SIGN_QUAD_HEIGHT = 2;

    struct
    {
        float x_center;
        float y_center;
    } _position;

    friend class QuadComponent;
    QuadComponent _quad;
};
