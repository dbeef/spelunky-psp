#pragma once

#include <cstdint>

// Intermediate compilation unit to include resource-compiled music and sound effects.
// This way recompilation is faster when changing something Audio-related.

namespace audio_bank
{
    const char* get_title_theme_wav();
    const char* get_cave_theme_wav();
    const char* get_jump_wav();
    const char* get_whip_wav();
    const char* get_entering_door_wav();
    const char* get_die_wav();
    const char* get_hurt_wav();
    const char* get_coin_wav();
};
