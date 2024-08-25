#include "audio/Audio.hpp"
#include "assets/Assets.hpp"
#include "assets/Paths.hpp"
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
    Mix_Chunk* item_throw = nullptr;
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
        case SFXType::THROW: Mix_PlayChannel(FIRST_FREE_CHANNEL, _handles->item_throw, 0); break;
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

    // TODO: Internal map of handles to SFXType/Path to easily iterate over all handles and assert/release/play
    #define LOAD(HANDLE, PATH) HANDLE = Mix_QuickLoad_WAV(Assets::instance().get_ptr<Uint8*>(PATH)); assert(HANDLE);
    LOAD(_handles->title_music, Paths::Audio::TITLE_THEME);
    LOAD(_handles->cave_music, Paths::Audio::CAVE_THEME);
    LOAD(_handles->jump, Paths::Audio::JUMP);
    LOAD(_handles->whip, Paths::Audio::WHIP);
    LOAD(_handles->entering_door, Paths::Audio::ENTERING_DOOR);
    LOAD(_handles->die, Paths::Audio::DIE);
    LOAD(_handles->hurt, Paths::Audio::HURT);
    LOAD(_handles->coin, Paths::Audio::COIN);
    LOAD(_handles->climb_1, Paths::Audio::CLIMB1);
    LOAD(_handles->climb_2, Paths::Audio::CLIMB2);
    LOAD(_handles->arrow_trap, Paths::Audio::ARROW_TRAP);
    LOAD(_handles->bat, Paths::Audio::BAT);
    LOAD(_handles->chest_open, Paths::Audio::CHEST_OPEN);
    LOAD(_handles->explosion, Paths::Audio::EXPLOSION);
    LOAD(_handles->gem, Paths::Audio::GEM);
    LOAD(_handles->hit, Paths::Audio::HIT);
    LOAD(_handles->jetpack, Paths::Audio::JETPACK);
    LOAD(_handles->pickup, Paths::Audio::PICKUP);
    LOAD(_handles->shotgun, Paths::Audio::SHOTGUN);
    LOAD(_handles->kiss, Paths::Audio::KISS);
    LOAD(_handles->item_throw, Paths::Audio::ITEM_THROW);
    #undef LOAD
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
    Mix_FreeChunk(_handles->item_throw); _handles->item_throw = nullptr;
}
