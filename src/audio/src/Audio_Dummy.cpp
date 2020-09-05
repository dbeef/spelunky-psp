#include "audio/Audio.hpp"
#include "logger/log.h"

REGISTER_SINGLETON_INSTANCE(Audio)

struct Audio::Handles
{
};

Audio::Audio() : _handles(std::make_unique<Audio::Handles>()) {}
Audio::~Audio() = default; // For pimpl

bool Audio::setup_audio()
{
    log_info("Entering Audio::setup_audio (dummy)");
    log_info("Exiting Audio::setup_audio (dummy), success.");
    return true;
}

void Audio::tear_down_audio()
{
}

void Audio::play(MusicType music)
{
}

void Audio::play(SFXType sfx)
{
}

void Audio::stop()
{
}

void Audio::pause()
{
}

void Audio::unpause()
{
}

void Audio::load()
{
    log_info("Loading music and sound effects (dummy).");
}

void Audio::unload()
{
    log_info("Unloading music and sound effects (dummy).");
}
