#pragma once

#include <memory>

#include "components/TextComponent.hpp"
#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "viewport/Viewport.hpp"

class HUD : public GameEntity
{
public:

    explicit HUD(std::shared_ptr<Viewport>);

    void update(uint32_t delta_time_ms) override;

    void set_hearts_count(uint32_t hearts);
    void set_ropes_count(uint32_t ropes);
    void set_bombs_count(uint32_t bombs);
    void set_dollars_count(uint32_t dollars);

private:

    static constexpr float ICONS_OFFSET_WORLD_UNITS = 1.5f;
    static constexpr float ICON_SIZE_WORLD_UNITS = 0.5f;

    std::shared_ptr<Viewport> _viewport;

    struct
    {
        QuadComponent heart = {TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS};
        QuadComponent dollar = {TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS};
        QuadComponent ropes = {TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS};
        QuadComponent bombs = {TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS};
        QuadComponent hold_item = {TextureType::HUD, Renderer::EntityType::SCREEN_SPACE, ICON_SIZE_WORLD_UNITS, ICON_SIZE_WORLD_UNITS};
    } _quads;

    struct
    {
        TextComponent hearts;
        TextComponent dollars;
        TextComponent ropes;
        TextComponent bombs;
    } _texts;
};
