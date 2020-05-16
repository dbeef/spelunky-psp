//
// Created by dbeef on 7/7/19.
//

#ifndef SPELUNKY_PSP_CONTEXT_HPP
#define SPELUNKY_PSP_CONTEXT_HPP

#include "time/Timestep.hpp"
#include <functional>

class Video {

public:

    Video() : _timestep(60) {};

    static void init();

    static void dispose();

    static Video &instance();

    float get_aspect() const;

    uint16_t get_window_width() const;

    uint16_t get_window_height() const;

    bool setup_gl();

    inline uint32_t get_delta_time() const { return _timestep.get_delta_ms(); }

    void tear_down_gl();

    void run_loop(const std::function<void(uint32_t delta_time_ms)> &loop_callback);

    inline void swap_buffers() const;

private:

    uint16_t _width = 0;
    uint16_t _height = 0;
    float _aspect = 0;

    Timestep _timestep;
    static Video *_instance;
};

#endif //SPELUNKY_PSP_CONTEXT_HPP
