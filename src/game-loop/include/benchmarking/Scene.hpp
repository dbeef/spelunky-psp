#pragma once

#include "ScreenSpaceCamera.hpp"
#include "ModelViewCamera.hpp"

#include <cstdint>

// TODO: Maybe I could make use of this Scene abstraction out of benchmarking context?
class Scene
{
public:
    virtual void update(const FrameStats&, ModelViewCamera&, ScreenSpaceCamera&) = 0;
    virtual void spawn_entities() const = 0;
    virtual const char* scene_title() const = 0;
};
