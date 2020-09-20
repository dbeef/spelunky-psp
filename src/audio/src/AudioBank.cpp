#include "AudioBank.hpp"

namespace title_wav
{
    #include "generated/title.wav.hpp"
}

namespace cave_wav
{
    #include "generated/cave.wav.hpp"
}

namespace jump_wav
{
    #include "generated/jump.wav.hpp"
}

namespace whip_wav
{
    #include "generated/whip.wav.hpp"
}

namespace entering_door_wav
{
    #include "generated/entering_door.wav.hpp"
}

namespace die_wav
{
    #include "generated/die.wav.hpp"
}

namespace hurt_wav
{
    #include "generated/hurt.wav.hpp"
}

namespace coin_wav
{
    #include "generated/coin.wav.hpp"
}

const char *audio_bank::get_title_theme_wav()
{
    return &title_wav::data[0];
}

const char *audio_bank::get_cave_theme_wav()
{
    return &cave_wav::data[0];
}

const char *audio_bank::get_jump_wav()
{
    return &jump_wav::data[0];
}

const char *audio_bank::get_whip_wav()
{
    return &whip_wav::data[0];
}

const char *audio_bank::get_entering_door_wav()
{
    return &entering_door_wav::data[0];
}

const char *audio_bank::get_die_wav()
{
    return &die_wav::data[0];
}

const char *audio_bank::get_hurt_wav()
{
    return &hurt_wav::data[0];
}

const char *audio_bank::get_coin_wav()
{
    return &coin_wav::data[0];
}
