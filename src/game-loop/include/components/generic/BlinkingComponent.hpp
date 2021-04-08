#pragma once

class BlinkingComponent
{
public:

    BlinkingComponent() = default;
    explicit BlinkingComponent(int interval_ms, int total_time_ms, bool remove_when_done)
        : _interval_ms(interval_ms)
        , _total_time_ms(total_time_ms)
        , _remove_when_done(remove_when_done)
    {}

    void update(std::uint32_t delta_time_ms)
    {
        _timer_ms += delta_time_ms;
        _timer_total_ms += delta_time_ms;

        if (_timer_ms > _interval_ms * 2)
        {
            _timer_ms = 0;
        }
    }

    bool is_transparent() const
    {
        return _timer_ms <= _interval_ms;
    }

    bool is_for_removal() const
    {
        return _remove_when_done && _timer_total_ms >= _total_time_ms;
    }

    void reset_timer()
    {
        _timer_total_ms = 0;
    }

    void set_original_quad_width(float width) { _original_quad_width = width; }
    void set_original_quad_height(float height) { _original_quad_height = height; }

    float get_original_quad_width() const { return _original_quad_width; }
    float get_original_quad_height() const { return _original_quad_height; }

private:
    int _interval_ms = 100;
    int _total_time_ms = 1000;
    bool _remove_when_done = true;

    int _timer_total_ms = 0;
    int _timer_ms = 0;

    // As blinking is achieved through periodic dimension change to infinitely small:
    float _original_quad_width = 0.0f;
    float _original_quad_height = 0.0f;
};
