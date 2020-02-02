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

Input::Input() : _left(false), _right(false), _up(false), _down(false), _exit(false)
{

}
