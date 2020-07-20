#include "main-dude/states/MainDudeDeadState.hpp"
#include "main-dude/MainDude.hpp"
#include "Input.hpp"

void MainDudeDeadState::enter(MainDude& main_dude)
{
    main_dude._animation.stop();
    main_dude._quad.frame_changed(MainDudeSpritesheetFrames::DEAD);
    // TODO: Change physical size of the main dude (about half of the height when dead)
    // TODO: Change animation frame depending on the bottom collision.
}

MainDudeBaseState* MainDudeDeadState::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // Update components:

    main_dude._physics.update(main_dude, delta_time_ms);
    main_dude._quad.update(main_dude.get_x_pos_center(), main_dude.get_y_pos_center(), !main_dude._other.facing_left);

    return this;
}

MainDudeBaseState *MainDudeDeadState::handle_input(MainDude& main_dude, const Input &input)
{
    return this;
}
