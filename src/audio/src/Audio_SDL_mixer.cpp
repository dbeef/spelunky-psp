#include "audio/Audio.hpp"
#include "AudioBank.hpp"
#include "logger/log.h"

// FIXME: Forcing SDL2; move all platforms to SDL2 (Windows left)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <type_traits>
#include <cassert>

REGISTER_SINGLETON_INSTANCE(Audio)

struct Audio::Handles
{
    Mix_Chunk* title_music = nullptr;
    Mix_Chunk* cave_music = nullptr;
    Mix_Chunk* jump = nullptr;
    Mix_Chunk* whip = nullptr;
    Mix_Chunk* entering_door = nullptr;
    Mix_Chunk* die = nullptr;
    Mix_Chunk* hurt = nullptr;
    Mix_Chunk* coin = nullptr;
    Mix_Chunk* climb_1 = nullptr;
    Mix_Chunk* climb_2 = nullptr;
    Mix_Chunk* arrow_trap = nullptr;
    Mix_Chunk* bat = nullptr;
    Mix_Chunk* chest_open = nullptr;
    Mix_Chunk* explosion = nullptr;
    Mix_Chunk* gem = nullptr;
    Mix_Chunk* hit = nullptr;
    Mix_Chunk* jetpack = nullptr;
    Mix_Chunk* pickup = nullptr;
    Mix_Chunk* shotgun = nullptr;
    Mix_Chunk* kiss = nullptr;
    Mix_Chunk* _throw = nullptr;
};

Audio::Audio() : _handles(std::make_unique<Audio::Handles>()) {}
Audio::~Audio() = default; // For pimpl

namespace
{
    namespace SpelunkyPSP_AudioFormat
    {
        // Some less-precise sampling frequency (i.e 22050 Hz) would be a better trade-off
        // between binary size and quality, but PSP refuses to open an audio device with
        // any sampling rate other than 44100 Hz due to hardware constraints.
        constexpr static int SAMPLING_FREQUENCY = 44100;
        // Same as in case of sampling frequency - PSP won't allow any other encoding.
        constexpr static uint16_t DATA_FORMAT = AUDIO_S16LSB;
        constexpr static int CHANNELS = 1;
    };

    namespace SpelunkyPSP_AudioDevice
    {
        constexpr static int CHANNELS = 4;
        constexpr static int CHUNK_SIZE = 2048;
    };

    const int FIRST_FREE_CHANNEL = -1;
    const int ALL_CHANNELS = -1;

    const char* map_format(uint16_t format)
    {
        if (format == AUDIO_U8)
        {
            return "AUDIO_U8";
        }
        else if (format == AUDIO_S8)
        {
            return "AUDIO_S8";
        }
        else if (format == AUDIO_U16LSB)
        {
            return "AUDIO_U16LSB";
        }
        else if (format == AUDIO_S16LSB)
        {
            return "AUDIO_S16LSB";
        }
        else if (format == AUDIO_U16MSB)
        {
            return "AUDIO_U16MSB";
        }
        else if (format == AUDIO_S16MSB)
        {
            return "AUDIO_S16MSB";
        }
        else
        {
            assert(false);
            return "UNKNOWN";
        }
    }

    bool device_capabilities_match_requested()
    {
        int freq, channel;
        uint16_t format;
        Mix_QuerySpec(&freq, &format, &channel);
        log_info("Opened audio device: %i Hz, %s, %i channel(s).", freq, map_format(format), channel);
        return freq == SpelunkyPSP_AudioFormat::SAMPLING_FREQUENCY &&
               channel == SpelunkyPSP_AudioFormat::CHANNELS &&
               format == SpelunkyPSP_AudioFormat::DATA_FORMAT;
    }
}

bool Audio::setup_audio()
{
    log_info("Entering Audio::setup_audio");

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        log_error("SDL_Init Error: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    if (Mix_OpenAudio(SpelunkyPSP_AudioFormat::SAMPLING_FREQUENCY,
                      SpelunkyPSP_AudioFormat::DATA_FORMAT,
                      SpelunkyPSP_AudioFormat::CHANNELS,
                      SpelunkyPSP_AudioDevice::CHUNK_SIZE) < 0)
    {
        log_error("Unable to open audio: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    // Assertion as Mix_OpenAudio does not guarantee that opened device will be of passed properties:
    if (!device_capabilities_match_requested())
    {
        log_error("Audio capabilities do not match requested.");
        return false;
    }

    if (Mix_AllocateChannels(SpelunkyPSP_AudioDevice::CHANNELS) < 0)
    {
        log_error("Unable to allocate mixing channels: %s", SDL_GetError());
        SDL_ClearError();
        return false;
    }

    log_info("Exiting Audio::setup_audio, success.");
    return true;
}

void Audio::tear_down_audio()
{
    Mix_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Audio::play(MusicType music)
{
    switch(music)
    {
        case MusicType::TITLE: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->title_music, 0); break;
        case MusicType::CAVE: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->cave_music, 0); break;
        default: assert(false); break;
    }
}

void Audio::play(SFXType sfx)
{
    switch(sfx)
    {
        case SFXType::MAIN_DUDE_JUMP: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->jump, 0); break;
        case SFXType::MAIN_DUDE_WHIP: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->whip, 0); break;
        case SFXType::MAIN_DUDE_ENTERING_DOOR: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->entering_door, 0); break;
        case SFXType::MAIN_DUDE_DIE: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->die, 0); break;
        case SFXType::MAIN_DUDE_HURT: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->hurt, 0); break;
        case SFXType::COIN: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->coin, 0); break;
        case SFXType::MAIN_DUDE_CLIMB_1: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->climb_1, 0); break;
        case SFXType::MAIN_DUDE_CLIMB_2: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->climb_2, 0); break;
        case SFXType::ARROW_TRAP: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->arrow_trap, 0); break;
        case SFXType::BAT: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->bat, 0); break;
        case SFXType::CHEST_OPEN: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->chest_open, 0); break;
        case SFXType::EXPLOSION: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->explosion, 0); break;
        case SFXType::GEM: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->gem, 0); break;
        case SFXType::HIT: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->hit, 0); break;
        case SFXType::JETPACK: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->jetpack, 0); break;
        case SFXType::PICKUP: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->pickup, 0); break;
        case SFXType::SHOTGUN: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->shotgun, 0); break;
        case SFXType::KISS: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->kiss, 0); break;
        case SFXType::THROW: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->_throw, 0); break;
        default: assert(false); break;
    }
}

void Audio::stop()
{
    Mix_HaltChannel(ALL_CHANNELS);
}

void Audio::pause()
{
    Mix_Pause(ALL_CHANNELS);
}

void Audio::unpause()
{
    Mix_Resume(ALL_CHANNELS);
}

void Audio::load()
{
    log_info("Loading music and sound effects.");

    _handles->title_music = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_title_theme_wav())));
    assert(_handles->title_music);
    _handles->cave_music = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_cave_theme_wav())));
    assert(_handles->cave_music);
    _handles->jump = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_jump_wav())));
    assert(_handles->jump);
    _handles->whip = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_whip_wav())));
    assert(_handles->whip);
    _handles->entering_door = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_entering_door_wav())));
    assert(_handles->entering_door);
    _handles->die = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_die_wav())));
    assert(_handles->die);
    _handles->hurt = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_hurt_wav())));
    assert(_handles->hurt);
    _handles->coin = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_coin_wav())));
    assert(_handles->coin);
    _handles->climb_1 = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_climb_1_wav())));
    assert(_handles->climb_1);
    _handles->climb_2 = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_climb_2_wav())));
    assert(_handles->climb_2);
    _handles->arrow_trap = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_arrow_trap_wav())));
    assert(_handles->arrow_trap);
    _handles->bat = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_bat_wav())));
    assert(_handles->bat);
    _handles->chest_open = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_chest_open_wav())));
    assert(_handles->chest_open);
    _handles->explosion = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_explosion_wav())));
    assert(_handles->explosion);
    _handles->gem = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_gem_wav())));
    assert(_handles->gem);
    _handles->hit = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_hit_wav())));
    assert(_handles->hit);
    _handles->jetpack = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_jetpack_wav())));
    assert(_handles->jetpack);
    _handles->pickup = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_pickup_wav())));
    assert(_handles->pickup);
    _handles->shotgun = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_shotgun_wav())));
    assert(_handles->shotgun);
    _handles->kiss = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_kiss_wav())));
    assert(_handles->kiss);
    _handles->_throw = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_throw_wav())));
    assert(_handles->_throw);
}

void Audio::unload()
{
    log_info("Unloading music and sound effects.");

    Mix_FreeChunk(_handles->title_music); _handles->title_music = nullptr;
    Mix_FreeChunk(_handles->cave_music); _handles->cave_music = nullptr;
    Mix_FreeChunk(_handles->jump); _handles->jump = nullptr;
    Mix_FreeChunk(_handles->whip); _handles->whip = nullptr;
    Mix_FreeChunk(_handles->entering_door); _handles->entering_door = nullptr;
    Mix_FreeChunk(_handles->die); _handles->die = nullptr;
    Mix_FreeChunk(_handles->hurt); _handles->hurt = nullptr;
    Mix_FreeChunk(_handles->coin); _handles->coin = nullptr;
    Mix_FreeChunk(_handles->climb_1); _handles->climb_1 = nullptr;
    Mix_FreeChunk(_handles->climb_2); _handles->climb_2 = nullptr;
    Mix_FreeChunk(_handles->arrow_trap); _handles->arrow_trap = nullptr;
    Mix_FreeChunk(_handles->bat); _handles->bat = nullptr;
    Mix_FreeChunk(_handles->chest_open); _handles->chest_open = nullptr;
    Mix_FreeChunk(_handles->explosion); _handles->explosion = nullptr;
    Mix_FreeChunk(_handles->gem); _handles->gem = nullptr;
    Mix_FreeChunk(_handles->hit); _handles->hit = nullptr;
    Mix_FreeChunk(_handles->jetpack); _handles->jetpack = nullptr;
    Mix_FreeChunk(_handles->pickup); _handles->pickup = nullptr;
    Mix_FreeChunk(_handles->shotgun); _handles->shotgun = nullptr;
    Mix_FreeChunk(_handles->kiss); _handles->kiss = nullptr;
    Mix_FreeChunk(_handles->_throw); _handles->_throw = nullptr;
}
