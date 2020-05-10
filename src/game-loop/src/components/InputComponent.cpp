#include "Camera.hpp"
#include "Input.hpp"
#include "components/InputComponent.hpp"
#include "main-dude/MainDude.hpp"

void InputComponent::update(MainDude& main_dude)
{
    Input& input = Input::instance();

    if (input.square()) // Left
    {
        main_dude._physics_component.add_velocity(-0.025f, 0.0f);
    }
    if (input.circle()) // Right
    {
        main_dude._physics_component.add_velocity(+0.025f, 0.0f);
    }
    if (input.triangle()) // Jump
    {
        if (main_dude._physics_component.is_bottom_collision())
        {
            main_dude._physics_component.add_velocity(0.0f, -0.18f);
        }
    }
}

