#include "components/InputComponent.hpp"
#include "Input.hpp"
#include "main-dude/MainDude.hpp"

void InputComponent::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    // TODO: Input polling timer
    Input& input = Input::instance();

    if (input.square())
    {
        main_dude._physics.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle())
    {
        main_dude._physics.add_velocity(+0.025f, 0.0f);
    }
    if (input.triangle())
    {
        if (main_dude._physics.is_bottom_collision())
        {
            main_dude._physics.add_velocity(0.0f, -0.18f);
        }
    }

}

