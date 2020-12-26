#include "main-dude/states/MainDudeLevelSummaryState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeLevelSummaryState::enter(MainDude& main_dude)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();

    assert(physics);
    assert(animation);

    physics->set_max_x_velocity(MainDude::DEFAULT_MAX_X_VELOCITY);
    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_0_FIRST),
                               static_cast<std::size_t>(MainDudeSpritesheetFrames::RUN_LEFT_5_LAST),
                               75, true);
}

MainDudeBaseState* MainDudeLevelSummaryState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    auto* physics = main_dude.get_physics_component();
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();

    assert(physics);
    assert(animation);
    assert(quad);

    physics->update(delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);
    animation->update(*quad, delta_time_ms);

    physics->set_velocity(0.095f, 0.0f);

    const auto* exit_tile = main_dude.is_overlaping_tile(MapTileType::EXIT);
    if (exit_tile)
    {
        physics->set_position(exit_tile->x + quad->get_quad_width() / 2, exit_tile->y + quad->get_quad_height() / 2);
        return &main_dude._states.exiting;
    }

    return this;
}

MainDudeBaseState *MainDudeLevelSummaryState::handle_input(MainDude& main_dude, const Input &input)
{
    // Input is blocked in this state.
    return this;
}
