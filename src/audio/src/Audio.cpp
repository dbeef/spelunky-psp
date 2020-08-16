#include "audio/Audio.hpp"
#include "AudioBank.hpp"
#include "logger/log.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <type_traits>

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
};

Audio::Audio() : _handles(std::make_unique<Audio::Handles>()) {}
Audio::~Audio() = default; // For pimpl

namespace
{
    namespace SpelunkyPSP_AudioFormat
    {
        constexpr static int SAMPLING_FREQUENCY = 44100;
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
    _handles->cave_music = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_cave_theme_wav())));
    _handles->jump = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_jump_wav())));
    _handles->whip = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_whip_wav())));
    _handles->entering_door = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_entering_door_wav())));
    _handles->die = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_die_wav())));
    _handles->hurt = Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>(const_cast<char*>(audio_bank::get_hurt_wav())));
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
}
