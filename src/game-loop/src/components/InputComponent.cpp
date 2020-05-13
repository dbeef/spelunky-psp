#include "components/InputComponent.hpp"
#include "Input.hpp"
#include "main-dude/MainDude.hpp"

namespace
{
    const uint32_t POLLING_INTERVAL = 16;
}

void InputComponent::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    _polling_timer += delta_time_ms;
    if (_polling_timer < POLLING_INTERVAL)
    {
        return;
    }

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

    _polling_timer = 0;
}
