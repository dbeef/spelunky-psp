//
// Created by xdbeef on 04.05.18.
//

#ifndef SPELUNKYDS_SPRITETYPE_H
#define SPELUNKYDS_SPRITETYPE_H

/**
 * CreatureType is focused on particular sprite, what it represents on screen, i.e blue ruby, or a single goldbar.
 * Helpful when it comes to count all killed npcs or collected loot.
 */
enum class CreatureType {
    MAIN_DUDE,
    BOMB,
    ROCK,
    JAR,
    SNAKE,
    ROPE,
    ROPE_ELEMENT,
    FLAME,
    WHIP,
    BLOOD,
    BAT,
    SPIDER,
    SPIKES,
    SHOTGUN,
    ARROW,
    SKELETON,
    CAVEMAN,
    DAMSEL,
    SHOPKEEPER,
    FALL_POOF,
    FAKE_SKELETON,
    CRATE,
    CHEST,
    SKULL,
    BONE,
    PISTOL,
    SPRING_SHOES,
    CAPE,
    COMPASS,
    GLOVE,
    SPIKE_SHOES,
    MITT,
    JETPACK,
    BULLET, BOULDER
};

#endif //SPELUNKYDS_SPRITETYPE_H
