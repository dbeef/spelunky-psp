#pragma once

#include <cassert>
#include <cstdint>

using ItemType_t = std::uint16_t;
enum class ItemType : ItemType_t
{
    ARROW = 0,
    BOMB,
    CAPE,
    CHEST,
    CRATE,
    JAR,
    JETPACK,
    PISTOL,
    ROCK,
    ROPE,
    SHOTGUN,
    SKULL,
    WHIP,
    BOMB_SPAWNER,
    ROPE_SPAWNER,
    WALLET,
    SPIKE_SHOES,
    SPRING_SHOES,
    MITT,
    GLOVE,
    COMPASS,
    BODY,
    BOMB_BAG,
    ROPE_PILE,
    GOLDEN_IDOL,
    FLARE,
    _SIZE
};

const char* to_string(ItemType item_type);