#include "game-loop/GameLoopBenchmarkingState.hpp"
#include "game-loop/GameLoop.hpp"
#include "benchmarking/FlaresScene.hpp"
#include "logger/log.h"
#include "EntityRegistry.hpp"
#include "components/generic/PositionComponent.hpp"
#include "system/RenderingSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/ParticleSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/AnimationSystem.hpp"
#include "system/DisposingSystem.hpp"

#include <fstream>
#include <vector>

namespace
{
    const char* filename = "benchmark_measurements.csv";
    const std::size_t time_per_scene_ms = 10000;
}

struct GameLoopBenchmarkingState::Context
{
    std::ofstream measurements_out{};
    std::size_t time_spent_in_current_scene_ms{};
    std::size_t current_scene_index{};
    std::vector<std::unique_ptr<Scene>> scenes{};
};

GameLoopBenchmarkingState::GameLoopBenchmarkingState() : _ctx(std::make_unique<GameLoopBenchmarkingState::Context>())
{
    _ctx->scenes.push_back(std::make_unique<FlaresScene>());
}

GameLoopBenchmarkingState::~GameLoopBenchmarkingState()
{
}

void GameLoopBenchmarkingState::save_frame(const FrameStats& last_frame)
{
    auto& measurements_out = _ctx->measurements_out;
    measurements_out << last_frame.total_delta_time_ms  << ',' << last_frame.game_loop_update_time_ms << '\n';
}

void GameLoopBenchmarkingState::open_file()
{
    auto& measurements_out = _ctx->measurements_out;
    measurements_out.open(filename, std::fstream::out);

    if (!measurements_out.is_open() || !measurements_out.good())
    {
        throw std::runtime_error("Benchmark failed to open file for writing");
    }

    measurements_out << "total_delta_time_ms,game_loop_update_time_ms\n";
}

GameLoopBaseState* GameLoopBenchmarkingState::update(GameLoop& game_loop)
{
    auto& last_frame = game_loop._last_frame;
    auto& registry = EntityRegistry::instance().get_registry();

    save_frame(last_frame);

    _ctx->time_spent_in_current_scene_ms += last_frame.total_delta_time_ms;
    if (_ctx->time_spent_in_current_scene_ms >= time_per_scene_ms)
    {
        _ctx->time_spent_in_current_scene_ms = 0;
        advance_scene();

        if (_ctx->current_scene_index >= _ctx->scenes.size())
        {
            game_loop._exit = true;
        }
        else
        {
            registry.clear();
            enter_scene(get_current_scene());
        }
    }

    if (!game_loop._exit)
    {
        auto &model_view_camera = game_loop._rendering_system->get_model_view_camera();
        auto &screen_space_camera = game_loop._rendering_system->get_screen_space_camera();

        get_current_scene().update(last_frame, model_view_camera, screen_space_camera);

        game_loop._rendering_system->update(last_frame.total_delta_time_ms);
        game_loop._scripting_system->update(last_frame.total_delta_time_ms);
        game_loop._particle_system->update(last_frame.total_delta_time_ms);
        game_loop._disposing_system->update(last_frame.total_delta_time_ms);
        game_loop._physics_system->update(last_frame.total_delta_time_ms);
        game_loop._animation_system->update(last_frame.total_delta_time_ms);
    }

    return this;
}

Scene& GameLoopBenchmarkingState::get_current_scene() const
{
    return *_ctx->scenes.at(_ctx->current_scene_index);
}

void GameLoopBenchmarkingState::advance_scene()
{
    _ctx->current_scene_index++;
}

void GameLoopBenchmarkingState::enter_scene(Scene& scene)
{
    log_info("Entering scene: %s", scene.scene_title());
    scene.spawn_entities();
}

void GameLoopBenchmarkingState::enter(GameLoop& game_loop)
{
    log_info("Entered GameLoopBenchmarkingState");
    open_file();
    enter_scene(get_current_scene());
}

void GameLoopBenchmarkingState::exit(GameLoop&)
{
    log_info("Exiting GameLoopBenchmarkingState");
    _ctx->measurements_out.close();
}
