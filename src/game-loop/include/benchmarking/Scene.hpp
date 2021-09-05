#pragma once

#include "ScreenSpaceCamera.hpp"
#include "ModelViewCamera.hpp"

#include <cstdint>

class FrameStats;

// TODO: Think of using this Scene abstraction out of benchmarking context
class Scene
{
public:
    virtual void update(const FrameStats&, ModelViewCamera&, ScreenSpaceCamera&) = 0;
    virtual void spawn_entities() = 0;
    virtual const char* scene_title() const = 0;
};
