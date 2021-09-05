#pragma once

#include "benchmarking/Scene.hpp"

class FlaresScene final : public Scene
{
public:
    void update(const FrameStats& frame_stats,
                ModelViewCamera& model_view_camera,
                ScreenSpaceCamera& screen_space_camera) override;
    void spawn_entities() override;
    const char* scene_title() const override;
private:
    int _camera_move_timer_ms{};
    int _flares_spawn_timer_ms{};
};
