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

namespace climb_1_wav
{
    #include "generated/climb1.wav.hpp"
}

namespace climb_2_wav
{
    #include "generated/climb2.wav.hpp"
}

namespace arrow_trap
{
    #include "generated/arrowtrap.wav.hpp"
}

namespace bat
{
    #include "generated/bat.wav.hpp"
}

namespace chest_open
{
    #include "generated/chestopen.wav.hpp"
}

namespace explosion
{
    #include "generated/explosion.wav.hpp"
}

namespace gem
{
    #include "generated/gem.wav.hpp"
}

namespace hit
{
    #include "generated/hit.wav.hpp"
}

namespace jetpack
{
    #include "generated/jetpack.wav.hpp"
}

namespace pickup
{
    #include "generated/pickup.wav.hpp"
}

namespace shotgun
{
    #include "generated/shotgun.wav.hpp"
}

namespace kiss
{
    #include "generated/kiss.wav.hpp"
}

namespace _throw
{
    #include "generated/throw.wav.hpp"
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

const char *audio_bank::get_climb_1_wav()
{
    return &climb_1_wav::data[0];
}

const char *audio_bank::get_climb_2_wav()
{
    return &climb_2_wav::data[0];
}

const char *audio_bank::get_arrow_trap_wav()
{
    return &arrow_trap::data[0];
}

const char *audio_bank::get_bat_wav()
{
    return &bat::data[0];
}

const char *audio_bank::get_chest_open_wav()
{
    return &chest_open::data[0];
}

const char *audio_bank::get_explosion_wav()
{
    return &explosion::data[0];
}

const char *audio_bank::get_gem_wav()
{
    return &gem::data[0];
}

const char *audio_bank::get_hit_wav()
{
    return &hit::data[0];
}

const char *audio_bank::get_jetpack_wav()
{
    return &jetpack::data[0];
}

const char *audio_bank::get_pickup_wav()
{
    return &pickup::data[0];
}

const char *audio_bank::get_shotgun_wav()
{
    return &shotgun::data[0];
}

const char *audio_bank::get_throw_wav()
{
    return &_throw::data[0];
}

const char *audio_bank::get_kiss_wav()
{
    return &kiss::data[0];
}
