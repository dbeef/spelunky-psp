#pragma once

#include "System.hpp"
#include "components/generic/InputComponent.hpp"

#include <vector>
#include <cstdarg>

class InputSystem final : public System
{
public:
    void update(std::uint32_t delta_time_ms) override;
private:

    std::vector<InputEvent> _input_events;

    void update_controllable_bodies();
    void update_items_open();
    void update_items_pick_up_put_down();
    void update_items_activate();

    template <class InputEvent>
    bool have_event(const std::vector<InputEvent>& events, InputEvent event);

    template <typename InputEvent, typename... Args>
    bool have_event(const std::vector<InputEvent>& events, InputEvent first, Args... args);

    template <class InputEvent>
    bool consume(InputEvent arg);

    template <typename InputEvent, typename... Args>
    bool consume(InputEvent first, Args... args);
};
