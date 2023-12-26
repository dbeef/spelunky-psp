#pragma once

#include "patterns/Singleton.hpp"
#include "Toggle.hpp"
#include "InputEvent.hpp"

#include <vector>

class Input : public Singleton<Input>
{
public:

    DELETE_COPY_MOVE_CONSTRUCTORS(Input)
    FRIEND_SINGLETON(Input)

    // TODO: Binding message function for each InputEvent + better way of mapping
    static const char* get_pause_binding_msg();
    static const char* get_death_request_binding_msg();
    static const char* get_quit_request_binding_msg();
    static const char* get_scores_request_binding_msg();
    static const char* get_accept_transaction_binding_msg();

    void poll();

    inline const Toggle& paused() const { return _toggles.paused; }
    inline const Toggle& death_requested() const { return _toggles.death_requested; }
    inline const Toggle& quit_requested() const { return _toggles.quit_requested; }
    inline const Toggle& left() const { return _toggles.left; }
    inline const Toggle& right() const { return _toggles.right; }
    inline const Toggle& up() const { return _toggles.up; }
    inline const Toggle& down() const { return _toggles.down; }
    inline const Toggle& jumping() const { return _toggles.jumping; }
    inline const Toggle& ducking() const { return _toggles.ducking; }
    inline const Toggle& running_fast() const { return _toggles.running_fast; }
    inline const Toggle& throwing() const { return _toggles.throwing; }
    inline const Toggle& out_bomb() const { return _toggles.out_bomb; }
    inline const Toggle& out_rope() const { return _toggles.out_rope; }
    inline const Toggle& purchase() const { return _toggles.purchase; }
    inline const Toggle& cheat_console() const { return _toggles.cheat_console; }

    inline const std::vector<InputEvent>& get_input_events() const { return _input_events; }

private:

    Input() = default;
    void fill_input_events();

    struct
    {
        Toggle paused{false};
        Toggle death_requested{false};
        Toggle quit_requested{false};
        Toggle left{false};
        Toggle right{false};
        Toggle up{false};
        Toggle down{false};
        Toggle jumping{false};
        Toggle ducking{false};
        Toggle running_fast{false};
        Toggle throwing{false};
        Toggle out_bomb{false};
        Toggle out_rope{false};
        Toggle purchase{false};
        Toggle cheat_console{false};
    } _toggles;

    std::vector<InputEvent> _input_events;
};
