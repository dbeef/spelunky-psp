//
// Created by xdbeef on 02.05.18.
//

#ifndef SPELUNKYDS_SPLASHSCREENTYPE_H
#define SPELUNKYDS_SPLASHSCREENTYPE_H

/**
 * Indicates type of a splash screen.
 * Every screen that is not a game level is a splash screen and must be defined here.
 * There's a division between lower-screen splash screen and upper-screen splash screen,
 * as they may be composed of different tiles.
 */
enum class SplashScreenType
{
    LEVEL_SUMMARY,
    SCORES,
    MAIN_MENU,
};

#endif //SPELUNKYDS_SPLASHSCREENTYPE_H
