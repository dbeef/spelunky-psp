#include <cassert>

#include "Input.hpp"

Input *Input::_input = nullptr;

Input &Input::instance()
{
    assert(_input);
    return *_input;
}

void Input::init()
{
    assert(!_input);
    _input = new Input();
}

void Input::dispose()
{
    assert(_input);
    delete _input;
    _input = nullptr;
}
