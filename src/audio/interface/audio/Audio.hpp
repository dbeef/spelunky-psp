#pragma once

#include "patterns/Singleton.hpp"
#include "audio/MusicType.hpp"
#include "audio/SFXType.hpp"

#include <memory>

class Audio : public Singleton<Audio>
{
public:

    ~Audio();

    DELETE_COPY_MOVE_CONSTRUCTORS(Audio)
    FRIEND_SINGLETON(Audio)

    bool setup_audio();
    void load();
    void unload();
    void tear_down_audio();

    void play(MusicType);
    void play(SFXType);
    void stop();
    void pause();
    void unpause();

private:

    Audio();
    struct Handles;
    std::unique_ptr<Handles> _handles;
};
