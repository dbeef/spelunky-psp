#include "Input.hpp"

REGISTER_SINGLETON_INSTANCE(Input)

void Input::fill_input_events()
{
    _input_events.clear();

    if (paused().value()) _input_events.push_back(InputEvent::PAUSED);
    if (paused().value() && paused().changed()) _input_events.push_back(InputEvent::PAUSED_PRESSED);

    if (death_requested().value()) _input_events.push_back(InputEvent::DEATH_REQUESTED);
    if (death_requested().value() && death_requested().changed()) _input_events.push_back(InputEvent::DEATH_REQUESTED_PRESSED);

    if (quit_requested().value()) _input_events.push_back(InputEvent::QUIT_REQUESTED);
    if (quit_requested().value() && quit_requested().changed()) _input_events.push_back(InputEvent::QUIT_REQUESTED_PRESSED);

    if (left().value()) _input_events.push_back(InputEvent::LEFT);
    if (left().value() && left().changed()) _input_events.push_back(InputEvent::LEFT_PRESSED);

    if (right().value()) _input_events.push_back(InputEvent::RIGHT);
    if (right().value() && right().changed()) _input_events.push_back(InputEvent::RIGHT_PRESSED);

    if (up().value()) _input_events.push_back(InputEvent::UP);
    if (up().value() && up().changed()) _input_events.push_back(InputEvent::UP_PRESSED);

    if (down().value()) _input_events.push_back(InputEvent::DOWN);
    if (down().value() && up().changed()) _input_events.push_back(InputEvent::DOWN_PRESSED);

    if (jumping().value()) _input_events.push_back(InputEvent::JUMPING);
    if (jumping().value() && jumping().changed()) _input_events.push_back(InputEvent::JUMPING_PRESSED);

    if (ducking().value()) _input_events.push_back(InputEvent::DUCKING);
    if (ducking().value() && ducking().changed()) _input_events.push_back(InputEvent::DUCKING_PRESSED);

    if (running_fast().value()) _input_events.push_back(InputEvent::RUNNING_FAST);
    if (running_fast().value() && running_fast().changed()) _input_events.push_back(InputEvent::RUNNING_FAST_PRESSED);

    if (throwing().value()) _input_events.push_back(InputEvent::THROWING);
    if (throwing().value() && throwing().changed()) _input_events.push_back(InputEvent::THROWING_PRESSED);

    if (purchase().value()) _input_events.push_back(InputEvent::ACCEPT_TRANSACTION);
    if (purchase().value() && purchase().changed()) _input_events.push_back(InputEvent::ACCEPT_TRANSACTION_PRESSED);

    if (out_bomb().value()) _input_events.push_back(InputEvent::OUT_BOMB);
    if (out_bomb().value() && out_bomb().changed()) _input_events.push_back(InputEvent::OUT_BOMB_PRESSED);

    if (out_rope().value()) _input_events.push_back(InputEvent::OUT_ROPE);
    if (out_rope().value() && out_rope().changed()) _input_events.push_back(InputEvent::OUT_ROPE_PRESSED);
}
