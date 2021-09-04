#pragma once

#include "game-loop/GameLoopBaseState.hpp"

#include <memory>

class GameLoop;
class Scene;
struct FrameStats;

class GameLoopBenchmarkingState : public GameLoopBaseState
{
public:
    struct Context;
    GameLoopBenchmarkingState();
    ~GameLoopBenchmarkingState();

    GameLoopBaseState* update(GameLoop&) override;
    void enter(GameLoop&) override;
    void exit(GameLoop&) override;
private:
    void save_frame(const FrameStats&);
    void open_file();
    void enter_scene(Scene&);
    Scene& get_current_scene() const;
    void advance_scene();
    std::unique_ptr<GameLoopBenchmarkingState::Context> _ctx;
};
