#include "main-dude/states/MainDudeStanding.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

#include <cstring>
#include <main-dude/MainDude.hpp>

MainDudeStanding::MainDudeStanding()
{

}

void MainDudeStanding::enter(MainDude& main_dude)
{
    main_dude._quad_component.frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);
}

MainDudeBaseState *MainDudeStanding::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    Input& input = Input::instance();

    // TODO: Input polling timer
    if (input.square())
    {
        main_dude._physics_component.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics_component.add_velocity(+0.025f, 0.0f);
    }
    if (input.triangle())
    {
        if (main_dude._physics_component.is_bottom_collision())
        {
            main_dude._physics_component.add_velocity(0.0f, -0.18f);
        }
    }

    main_dude._physics_component.update(main_dude, delta_time_ms);
    main_dude._quad_component.update(main_dude, delta_time_ms);

    const auto velocity_x = main_dude._physics_component.get_x_velocity();
    if (velocity_x != 0.0f)
    {
        return &main_dude._states.running;
    }

    main_dude._quad_component.frame_changed(MainDudeSpritesheetFrames::STAND_LEFT);
    return this;
}
