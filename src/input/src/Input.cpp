#include <cassert>
#include <logger/log.h>

#include "Input.hpp"
#include "Camera.hpp"

Input *Input::_Input = nullptr;

Input &Input::instance()
{
    assert(_Input);
    return *_Input;
}

void Input::init()
{
    assert(!_Input);
    _Input = new Input();
}

void Input::dispose()
{
    assert(_Input);
    delete _Input;
}

Input::Input() : _left(false), _right(false), _up(false), _down(false)
{

}

void Input::process(SDL_Event &e)
{
    auto &camera = Camera::instance();

    if (e.type == SDL_EventType::SDL_KEYDOWN)
    {
        auto &key_event = e.key;

        if (e.key.keysym.sym == SDLK_LEFT)
        {
            camera.setX(camera.getX() - 1);
        } else if (e.key.keysym.sym == SDLK_RIGHT)
        {
            camera.setX(camera.getX() + 1);
        } else if (e.key.keysym.sym == SDLK_UP)
        {
            camera.setY(camera.getY() - 1);
        } else if (e.key.keysym.sym == SDLK_DOWN)
        {
            camera.setY(camera.getY() + 1);
        }
    }
}
