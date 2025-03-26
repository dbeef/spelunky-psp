#include <utility>
#include <sstream>
#include <cstring>

#include "components/generic/PositionComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "Input.hpp"
#include "components/specialized/DeathOverlayComponent.hpp"
#include "EntityRegistry.hpp"

namespace
{
    const char* GAME_OVER_MSG = "GAME OVER";
}

DeathOverlayComponent::DeathOverlayComponent(std::shared_ptr<Viewport>  viewport) : _viewport(std::move(viewport))
{
}

void DeathOverlayComponent::launch()
{
    auto& registry = EntityRegistry::instance().get_registry();

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        text.set_scale(2.0f);
        text.set_text(GAME_OVER_MSG);
        text.set_yellow(true);
        position.x_center = (_viewport->get_width_world_units() / 2.0f) -
                            (text.get_width() / 2.0f) +
                            (text.get_font_width() / 2.0f);
        position.y_center = _viewport->get_height_world_units() * 0.25f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);

        _children.push_back(entity);
    }

    {
        auto entity = registry.create();
        TextComponent text;
        PositionComponent position;
        MeshComponent mesh;

        text.set_text(get_available_controls_msg());
        text.set_yellow(true);
        position.x_center = (_viewport->get_width_world_units() / 2.0f) -
                            (text.get_width() / 2.0f) +
                            (text.get_font_width() / 2.0f);
        position.y_center = _viewport->get_height_world_units() * 0.45f;

        registry.emplace<TextComponent>(entity, text);
        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<MeshComponent>(entity, mesh);

        _children.push_back(entity);
    }
}

void DeathOverlayComponent::update()
{
    if (_disabled_input)
    {
        return;
    }

    const auto& input = Input::instance();
    if (input.purchase().value() && input.purchase().changed())
    {
        _scores_requested = true;
    }
}

std::string DeathOverlayComponent::get_available_controls_msg() const
{
    std::stringstream out;
    out << "PRESS ";
    out << Input::get_scores_request_binding_msg();
    out << " FOR HIGH SCORES.";
    return out.str();
}

void DeathOverlayComponent::disable_input()
{
    _disabled_input = true;
}

void DeathOverlayComponent::enable_input()
{
    _disabled_input = false;
}
