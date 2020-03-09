//
// Created by dbeef on 7/7/19.
//

#ifndef SPELUNKY_PSP_CONTEXT_HPP
#define SPELUNKY_PSP_CONTEXT_HPP

#include <functional>

class Video {

public:

    static void init();

    static void dispose();

    static Video &instance();

    static float get_aspect();

    static uint16_t get_window_width();

    static uint16_t get_window_height();

    bool setup_gl();

    void tear_down_gl();

    void run_loop(std::function<void()> &loop_callback);

    inline void swap_buffers() const;

private:

    static Video *_instance;
};

#endif //SPELUNKY_PSP_CONTEXT_HPP
