#include "main-dude/states/MainDudeExitingState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"
#include "audio/Audio.hpp"

void MainDudeExitingState::exit(MainDude& main_dude)
{
    main_dude._other.entered_door = false;
}

void MainDudeExitingState::enter(MainDude& main_dude)
{
    auto* animation = main_dude.get_animation_component();
    assert(animation);

    Audio::instance().play(SFXType::MAIN_DUDE_ENTERING_DOOR);
    animation->start(static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_0_FIRST),
                     static_cast<std::size_t>(MainDudeSpritesheetFrames::EXITING_LEFT_15_LAST),
                     75, false);
}

MainDudeBaseState* MainDudeExitingState::update(MainDude& main_dude, World* world, uint32_t delta_time_ms)
{
    auto* animation = main_dude.get_animation_component();
    auto* quad = main_dude.get_quad_component();
    auto* physics = main_dude.get_physics_component();

    assert(animation);
    assert(quad);
    assert(physics);

    animation->update(*quad, delta_time_ms);
    quad->update(physics->get_x_position(), physics->get_y_position(), !main_dude._other.facing_left);

    if (animation->is_finished())
    {
        main_dude._other.entered_door = true;
    }

    return this;
}

MainDudeBaseState *MainDudeExitingState::handle_input(MainDude& main_dude, World* world, const Input &input)
{
    return this;
}
