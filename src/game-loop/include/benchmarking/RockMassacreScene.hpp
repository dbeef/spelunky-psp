#pragma once

#include "benchmarking/Scene.hpp"

class RockMassacreScene final : public Scene
{
public:
    void update(const FrameStats& frame_stats,
                ModelViewCamera& model_view_camera,
                ScreenSpaceCamera& screen_space_camera) override;
    void spawn_entities() override;
    const char* scene_title() const override;
private:
    const int _rock_spawn_interval_ms = 1000;
    int _rock_spawn_timer_ms{_rock_spawn_interval_ms};
};
