#pragma once

#include "benchmarking/Scene.hpp"
#include <entt/entt.hpp>

class BombsScene final : public Scene
{
public:
    void update(const FrameStats &frame_stats,
                ModelViewCamera &model_view_camera,
                ScreenSpaceCamera &screen_space_camera) override;
    void spawn_entities() override;
    const char *scene_title() const override;
private:
    int _bombs_spawn_timer_ms{};
    entt::entity _rock{entt::null};
};
