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

    static uint16_t getWindowWidth();

    static uint16_t getWindowHeight();

    bool setupGL();

    void tearDownGL();

    void runLoop(std::function<void()> &loopCallback);

    inline void swapBuffers() const;

private:

    static Video *_instance;
};

#endif //SPELUNKY_PSP_CONTEXT_HPP
