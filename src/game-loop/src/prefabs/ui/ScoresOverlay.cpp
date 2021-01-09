#include "EntityRegistry.hpp"
#include "prefabs/ui/ScoresOverlay.hpp"
#include "prefabs/ui/Text.hpp"

#include "components/generic/PositionComponent.hpp"
#include "components/generic/TextComponent.hpp"

#include <sstream>
#include <iomanip>

namespace
{
    const char* TOP_DEFILERS_MSG = "TOP DEFILERS";
    const char* STATISTICS_MSG = "STATISTICS";
    const char* MONEY_MSG = "MONEY:";
    const char* KILLS_MSG = "KILLS:";
    const char* SAVES_MSG = "SAVES:";
    const char* PLAYS_MSG = "PLAYS:";
    const char* DEATHS_MSG = "DEATHS:";
    const char* WINS_MSG = "WINS:";

    // TODO: Pass actual numbers.
    const int MONEY = 0;
    const int KILLS = 0;
    const int SAVES = 0;
    const int PLAYS = 0;
    const int DEATHS = 0;
    const int WINS = 0;

    std::string format(const char* msg, int value)
    {
        std::stringstream out;
        out << std::setw(8) << std::left;
        out << msg << " " << value;
        return out.str();
    }
}

void prefabs::ScoresOverlay::create(const std::shared_ptr<Viewport> &viewport)
{
    auto& registry = EntityRegistry::instance().get_registry();
    
    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_yellow(true);
        text.set_text(TOP_DEFILERS_MSG);

        position.x_center = (viewport->get_width_world_units() * 0.625f) -
                            (text.get_width() / 2.0f) +
                            (text.get_font_width() / 2.0f);
        position.y_center = viewport->get_height_world_units() * 0.15f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(format(MONEY_MSG, MONEY));

        position.x_center = viewport->get_width_world_units() * 0.4f;
        position.y_center = viewport->get_height_world_units() * 0.23f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(format(KILLS_MSG, KILLS));

        position.x_center = viewport->get_width_world_units() * 0.4f;
        position.y_center = viewport->get_height_world_units() * 0.31f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(format(SAVES_MSG, SAVES));

        position.x_center = viewport->get_width_world_units() * 0.4f;
        position.y_center = viewport->get_height_world_units() * 0.39f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(STATISTICS_MSG);
        text.set_yellow(true);

        position.x_center = (viewport->get_width_world_units() * 0.625f) -
                            (text.get_width() / 2.0f) +
                            (text.get_font_width() / 2.0f);
        position.y_center = viewport->get_height_world_units() * 0.47f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(format(PLAYS_MSG, PLAYS));

        position.x_center = viewport->get_width_world_units() * 0.4f;
        position.y_center = viewport->get_height_world_units() * 0.55f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(format(DEATHS_MSG, DEATHS));

        position.x_center = viewport->get_width_world_units() * 0.4f;
        position.y_center = viewport->get_height_world_units() * 0.63f;
    }

    {
        auto entity = prefabs::Text::create();

        auto &text = registry.get<TextComponent>(entity);
        auto &position = registry.get<PositionComponent>(entity);

        text.set_text(format(WINS_MSG, WINS));

        position.x_center = viewport->get_width_world_units() * 0.4f;
        position.y_center = viewport->get_height_world_units() * 0.71f;
    }
}
