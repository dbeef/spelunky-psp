#pragma once

#include "components/QuadComponent.hpp"
#include "GameObject.hpp"
#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "Point2D.hpp"

class HUD : public GameObject
{
public:

    HUD(float pos_x, float pos_y)
            :_heart_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _dollar_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _ropes_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
            , _bombs_quad(TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_WIDTH_PIXELS, ICON_HEIGHT_PIXELS)
    {
        _heart_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HEART);
        _dollar_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::DOLLAR_SIGN);
        _ropes_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::ROPE_ICON);
        _bombs_quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::BOMB_ICON);

        const float icons_offset_pixels = Video::instance().get_window_width() * 0.1f;

        _heart_center.x = pos_x + (icons_offset_pixels * 0);
        _dollar_center.x = pos_x + (icons_offset_pixels * 1);
        _ropes_center.x = pos_x + (icons_offset_pixels * 2);
        _bombs_center.x = pos_x + (icons_offset_pixels * 3);

        _heart_center.y = pos_y;
        _dollar_center.y = pos_y;
        _ropes_center.y = pos_y;
        _bombs_center.y = pos_y;
    }

    void update(uint32_t delta_time_ms) override
    {
        _heart_quad.update(_heart_center.x, _heart_center.y);
        _dollar_quad.update(_dollar_center.x, _dollar_center.y);
        _ropes_quad.update(_ropes_center.x, _ropes_center.y);
        _bombs_quad.update(_bombs_center.x, _bombs_center.y);
    }

private:

    const float ICON_WIDTH_PIXELS = 16;
    const float ICON_HEIGHT_PIXELS = 16;

    Point2D _heart_center;
    Point2D _dollar_center;
    Point2D _ropes_center;
    Point2D _bombs_center;

    QuadComponent _heart_quad;
    QuadComponent _dollar_quad;
    QuadComponent _ropes_quad;
    QuadComponent _bombs_quad;
};
