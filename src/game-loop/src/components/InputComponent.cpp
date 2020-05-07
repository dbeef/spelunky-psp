#include "Camera.hpp"
#include "Input.hpp"
#include "components/InputComponent.hpp"
#include "MainDude.hpp"

void InputComponent::update(MainDude& main_dude)
{
    Input& input = Input::instance();

    if (input.square())
    {
        main_dude._x = main_dude._x - 0.1f;
    }
    if (input.circle())
    {
        main_dude._x = main_dude._x + 0.1f;
    }
    if (input.triangle())
    {
        main_dude._y = main_dude._y - 0.1f;
    }
    if (input.cross())
    {
        main_dude._y = main_dude._y + 0.1f;
    }
}

