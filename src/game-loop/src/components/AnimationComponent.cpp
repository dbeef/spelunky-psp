#include "components/AnimationComponent.hpp"
#include "main-dude/MainDude.hpp"

void AnimationComponent::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    if (!_running)
    {
        return;
    }

    _timer_ms += delta_time_ms;
    if (_timer_ms < _time_per_frame_ms)
    {
        return;
    }

    if (_current_frame_index == _end_frame_index)
    {
        if (_loop)
        {
            _current_frame_index = _start_frame_index;
        }
        else
        {
            _finished = true;
            _running = false;
        }
    }
    else
    {
        _current_frame_index++;
    }

    main_dude._quad_component.frame_changed(_current_frame_index);
    _timer_ms = 0;
}

void AnimationComponent::start(std::size_t start_frame, std::size_t end_frame, uint32_t time_per_frame_ms, bool loop)
{
    _current_frame_index = start_frame;
    _start_frame_index = start_frame;
    _end_frame_index = end_frame;
    _loop = loop;
    _time_per_frame_ms = time_per_frame_ms;
    _timer_ms = 0;

    _running = true;
    _finished = false;
}
