//
// Created by xdbeef on 11.04.18.
//

#ifndef SPELUNKYDS_SPRITETYPES_H
#define SPELUNKYDS_SPRITETYPES_H

/**
 * SpritesheetType is focused on file, what it contains, i.e main dude animations and whips.
 * It helps when it comes to manage multiple palettes - see OamManager.cpp
 */
enum SpritesheetType{
    MAIN_DUDE,                  //0
    MENU_SIGNS_TUTORIAL_QUIT,   //1
    MENU_SIGNS_START_SCORES,    //2
    BOMB,                       //3
    BLOOD_ROCK_ROPE_POOF,       //4
    JAR,                        //5
    MONIEZ_GOLDBARS,            //6
    MONIEZ_RUBY,                //7
    SNAKE,                      //8
    PRE_WHIP,                   //11
    WHIP,                       //12
    HUD,                        //13
    SPELUNKY_TITLE,             //15
    SPELUNKY_COPYRIGHTS,        //16
    BAT_JETPACK,                //17
    SHOPKEEPER,                 //17
    SKELETON_SPIDER,            //18
    SPIKES_COLLECTIBLES,        //19
    SALEABLE,                   //20
    GOT_COLLECTIBLE,            //21
    CAVEMAN_DAMSEL,             //22
    ARROW,             //23
    NONE
};

#endif //SPELUNKYDS_SPRITETYPES_H
