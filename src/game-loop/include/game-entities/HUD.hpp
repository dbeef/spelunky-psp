#pragma once

#include <memory>

#include "components/TextComponent.hpp"
#include "GameEntity.hpp"
#include "components/QuadComponent.hpp"
#include "Point2D.hpp"
#include "viewport/Viewport.hpp"
#include "patterns/Observer.hpp"
#include "main-dude/MainDudeEvent.hpp"

class MainDude;

class HUD : public GameEntity, public Observer<MainDudeEvent>
{
public:

    HUD(std::shared_ptr<Viewport>, std::shared_ptr<MainDude> main_dude);
    ~HUD() override;

    void update(uint32_t delta_time_ms) override;
    void on_notify(MainDudeEvent) override;

private:

    static constexpr float ICONS_OFFSET_WORLD_UNITS = 1.5f;
    static constexpr float ICON_SIZE_WORLD_UNITS = 0.5f;

    std::shared_ptr<Viewport> _viewport;
    std::shared_ptr<MainDude> _main_dude;

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
