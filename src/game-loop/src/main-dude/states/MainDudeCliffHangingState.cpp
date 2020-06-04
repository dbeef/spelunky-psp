#include "LevelGenerator.hpp"
#include "main-dude/MainDude.hpp"
#include "main-dude/states/MainDudeCliffHangingState.hpp"
#include "Input.hpp"

void MainDudeCliffHangingState::enter(MainDude& main_dude)
{
    main_dude._physics.set_velocity(0.0f, 0.0f);
    main_dude._physics.disable_gravity();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::HANGING_LEFT);
}

MainDudeBaseState* MainDudeCliffHangingState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:
    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);

    return this;
}

MainDudeBaseState *MainDudeCliffHangingState::handle_input(MainDude& main_dude, const Input &input)
{
    if (input.circle())
    {
        main_dude._physics.add_velocity(0.0f, -MainDude::JUMP_SPEED);
        main_dude._physics.enable_gravity();
        return &main_dude._states.jumping;
    }

    return this;
}
