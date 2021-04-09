#include "components/specialized/PauseOverlayComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/PositionComponent.hpp"

#include "spritesheet-frames/HUDSpritesheetFrames.hpp"
#include "components/specialized/PauseOverlayComponent.hpp"
#include "Input.hpp"

#include <cstring>
#include <sstream>
#include <cassert>

namespace
{
    const char* PAUSED_MSG = "PAUSED";
}

void PauseOverlayComponent::update(entt::registry& registry)
{
    if (_disabled_input)
    {
        return;
    }

    const auto& input = Input::instance();

    if (input.paused().changed() && input.paused().value())
    {
        if (!_paused)
        {
            // Just paused:
            _paused = true;

            {
                auto entity = registry.create();

                PositionComponent position(_viewport->get_width_world_units() / 2.0f,
                                                _viewport->get_height_world_units() / 2.0f);
                QuadComponent quad(TextureType::HUD,
                                        _viewport->get_width_world_units(),
                                        _viewport->get_height_world_units());
                MeshComponent mesh;

                mesh.rendering_layer = RenderingLayer::LAYER_1_OVERLAY;
                mesh.camera_type = CameraType::SCREEN_SPACE;
                quad.frame_changed<HUDSpritesheetFrames>(HUDSpritesheetFrames::HALF_OPAQUE_TILE);

                registry.emplace<QuadComponent>(entity, quad);
                registry.emplace<PositionComponent>(entity, position);
                registry.emplace<MeshComponent>(entity, mesh);

                _children.push_back(entity);
            }

            {
                auto entity = registry.create();
                TextComponent text_pause;
                PositionComponent position;
                MeshComponent mesh;

                text_pause.set_layer(RenderingLayer::LAYER_0_OVERLAY_TEXT);
                text_pause.set_scale(2.0f);
                text_pause.set_text(PAUSED_MSG);
                position.x_center = (_viewport->get_width_world_units() / 2.0f) -
                                    (text_pause.get_width() / 2.0f) +
                                    (text_pause.get_font_width() / 2.0f);
                position.y_center = _viewport->get_height_world_units() * 0.8f;


                registry.emplace<TextComponent>(entity, text_pause);
                registry.emplace<PositionComponent>(entity, position);
                registry.emplace<MeshComponent>(entity, mesh);

                _children.push_back(entity);
            }

            {
                auto entity = registry.create();
                TextComponent text_controls;
                PositionComponent position;
                MeshComponent mesh;

                const std::string available_controls = get_available_controls_msg();

                text_controls.set_layer(RenderingLayer::LAYER_0_OVERLAY_TEXT);
                text_controls.set_text(available_controls);
                position.x_center = (_viewport->get_width_world_units() / 2.0f) -
                                    (text_controls.get_width() / 2.0f) +
                                    (text_controls.get_font_width() / 2.0f);
                position.y_center = _viewport->get_height_world_units() * 0.9f;

                registry.emplace<TextComponent>(entity, text_controls);
                registry.emplace<PositionComponent>(entity, position);
                registry.emplace<MeshComponent>(entity, mesh);

                _children.push_back(entity);
            }
        }
        else
        {
            // Just unpaused:

            _paused = false;
            _death_requested = false;
            _quit_requested = false;

            hide(registry);
        }
    }

    if (_paused)
    {
        // Currently in pause:
        _quit_requested = input.quit_requested().value();
        _death_requested = input.death_requested().value();
    }
}

std::string PauseOverlayComponent::get_available_controls_msg() const
{
    std::stringstream out;

    switch(_type)
    {
        case Type::SCORES:
        case Type::MAIN_MENU:
        {
            out << Input::get_pause_binding_msg() << "-RETURN ";
            out << Input::get_quit_request_binding_msg() << "-QUIT";
            break;
        }
        case Type::PLAYING:
        {
            out << Input::get_pause_binding_msg() << "-RETURN ";
            out << Input::get_death_request_binding_msg() << "-DIE ";
            out << Input::get_quit_request_binding_msg() << "-QUIT";
            break;
        }
        default:
        {
            assert(false);
            return "";
        }
    }

    return out.str();
}

PauseOverlayComponent::PauseOverlayComponent(std::shared_ptr<Viewport> viewport, Type pause_screen_type)
    : _viewport(std::move(viewport))
    , _type(pause_screen_type)
{
}

PauseOverlayComponent::PauseOverlayComponent(const PauseOverlayComponent& other)
    : _viewport(other._viewport)
    , _type(other._type)
{
}

void PauseOverlayComponent::hide(entt::registry& registry)
{
    for(auto& entity : _children)
    {
        registry.destroy(entity);
    }

    _children.clear();
}
