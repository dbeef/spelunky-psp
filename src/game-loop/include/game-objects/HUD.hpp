#pragma once

#include "MainMenuSpritesheetFrames.hpp"
#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "HUDSpritesheetFrames.hpp"

class HUD : public GameObject
{
public:

    HUD(float pos_x_center, float pos_y_center)
            :_heart_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, HUD_ICON_WIDTH_PIXELS, HUD_ICON_HEIGHT_PIXELS)
            , _dollar_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, HUD_ICON_WIDTH_PIXELS, HUD_ICON_HEIGHT_PIXELS)
            , _ropes_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, HUD_ICON_WIDTH_PIXELS, HUD_ICON_HEIGHT_PIXELS)
            , _bombs_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, HUD_ICON_WIDTH_PIXELS, HUD_ICON_HEIGHT_PIXELS)
    {
        _heart_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
        _dollar_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::DOLLAR_SIGN);
        _ropes_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::ROPE_ICON);
        _bombs_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::BOMB_ICON);

        const float icons_offset_pixels = Video::instance().get_window_width() * 0.1f;

        _heart_position.x_center = pos_x_center + (icons_offset_pixels * 0);
        _dollar_position.x_center = pos_x_center + (icons_offset_pixels * 1);
        _ropes_position.x_center = pos_x_center + (icons_offset_pixels * 2);
        _bombs_position.x_center = pos_x_center + (icons_offset_pixels * 3);

        _heart_position.y_center = pos_y_center;
        _dollar_position.y_center = pos_y_center;
        _ropes_position.y_center = pos_y_center;
        _bombs_position.y_center = pos_y_center;
    }

    void update(uint32_t delta_time_ms) override
    {
        _heart_quad.update(_heart_position.x_center, _heart_position.y_center);
        _dollar_quad.update(_dollar_position.x_center, _dollar_position.y_center);
        _ropes_quad.update(_ropes_position.x_center, _ropes_position.y_center);
        _bombs_quad.update(_bombs_position.x_center, _bombs_position.y_center);
    }

private:

    const float HUD_ICON_WIDTH_PIXELS = 16;
    const float HUD_ICON_HEIGHT_PIXELS = 16;

    // TODO: Introduce point/vec2d structure everywhere.
    struct Position
    {
        float x_center;
        float y_center;
    };

    Position _heart_position;
    Position _dollar_position;
    Position _ropes_position;
    Position _bombs_position;

    friend class QuadComponent;

    QuadComponent _heart_quad;
    QuadComponent _dollar_quad;
    QuadComponent _ropes_quad;
    QuadComponent _bombs_quad;
};
