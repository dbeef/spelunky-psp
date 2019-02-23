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

// TODO Get delta time since last call
// store a timestamp of last call to timer

static int my_tick_handler(){
    static int div;
    static bool tic;
    div++;
    //arrange to get more or less seconds...
    if(div > 12){
        div = 0;
        if(tic){
            pspDebugScreenPrintf("Tac!\n");
        }else{
            pspDebugScreenPrintf("Tic!\n");
        }
        tic = ~tic;
    }
    return -1;
}

class Timer {

public:

    SceSysTimerId timer;

    Timer(){

//        pspDebugScreenInit();
//        pspDebugScreenPrintf("SysTimer sample by Adrahil.\n");
//
//        timer = sceSTimerAlloc();
//        if(timer)
//        {
//            exit(0);
//        }

//        sceSTimerStartCount(timer);
//        sceSTimerSetHandler(timer, 4194303, my_tick_handler, 0);

//        sceKernelSleepThreadCB();
    }

};


#endif //SPELUNKYPSP_TIMER_H
