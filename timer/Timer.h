//
// Created by dbeef on 2/23/19.
//

#ifndef SPELUNKYPSP_TIMER_H
#define SPELUNKYPSP_TIMER_H

#include <pspthreadman.h>
#include <pspsystimer.h>
#include <pspdebug.h>
#include <pspuser.h>
#include <cstdlib>
#include <psprtc.h>
#include <stdio.h>

class Timer {

public:

    Timer(){
        tick_before = new u64(0);
    }

    ~Timer(){
        delete tick_before;
    }

    void update(){

        u32 tick_freq = sceRtcGetTickResolution();

        if (*tick_before == 0) {
            int result = sceRtcGetCurrentTick(tick_before);
        } else {
            u64 temp_tick;
            int result = sceRtcGetCurrentTick(&temp_tick);
            u64 diff = temp_tick - *tick_before;
            printf("Result:  %i \n", result);
            printf("Current tick :  %lu \n", temp_tick);
            printf("Diff:  %lu \n", diff);
            float delta = ((float) diff / tick_freq);
            printf("Delta:  %f \n", delta - last_delta);
            last_delta = delta;
        }
    }

    u64 *tick_before;
    float last_delta{};
};


#endif //SPELUNKYPSP_TIMER_H
