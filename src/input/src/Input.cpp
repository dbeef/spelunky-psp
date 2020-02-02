#include <cassert>

#include "Input.hpp"

Input* Input::_Input = nullptr;

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
