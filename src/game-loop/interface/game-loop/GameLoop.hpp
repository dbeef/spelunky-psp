#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "ModelViewCamera.hpp"
#include "ScreenSpaceCamera.hpp"
#include "video/Video.hpp"

#include "game-loop/GameLoopBaseState.hpp"
#include "game-loop/GameLoopLevelSummaryState.hpp"
#include "game-loop/GameLoopMainMenuState.hpp"
#include "game-loop/GameLoopPlayingState.hpp"
#include "game-loop/GameLoopStartedState.hpp"
#include "game-loop/GameLoopScoresState.hpp"
#include "game-loop/GameLoopBenchmarkingState.hpp"

#include <entt/entt.hpp>

struct FrameStats;
class MainDudeComponent;
class LevelSummaryTracker;
class Viewport;
class RenderingSystem;
class ScriptingSystem;
class PhysicsSystem;
class AnimationSystem;
class CollectibleSystem;
class InputSystem;
class DisposingSystem;
class ParticleSystem;
class ItemSystem;
class DamageSystem;
class ShoppingSystem;

class GameLoop
{
public:
    GameLoop(const std::shared_ptr<Viewport>&);
    std::function<bool(const FrameStats&)>& get();
private:

    friend class GameLoopBaseState;
    friend class GameLoopMainMenuState;
    friend class GameLoopPlayingState;
    friend class GameLoopStartedState;
    friend class GameLoopLevelSummaryState;
    friend class GameLoopScoresState;
    friend class GameLoopBenchmarkingState;

    struct
    {
        GameLoopMainMenuState main_menu;
        GameLoopPlayingState playing;
        GameLoopStartedState started;
        GameLoopLevelSummaryState level_summary;
        GameLoopScoresState scores;
        GameLoopBenchmarkingState benchmarking;
        GameLoopBaseState* current;
    } _states;

    bool _exit = false;
    std::uint32_t _time_elapsed_ms = 0;
    FrameStats _last_frame{};

    std::shared_ptr<PhysicsSystem> _physics_system;
    std::shared_ptr<AnimationSystem> _animation_system;
    std::shared_ptr<RenderingSystem> _rendering_system;
    std::shared_ptr<ScriptingSystem> _scripting_system;
    std::shared_ptr<CollectibleSystem> _collectible_system;
    std::shared_ptr<DisposingSystem> _disposing_system;
    std::shared_ptr<ParticleSystem> _particle_system;
    std::shared_ptr<InputSystem> _input_system;
    std::shared_ptr<ItemSystem> _item_system;
    std::shared_ptr<DamageSystem> _damage_system;
    std::shared_ptr<ShoppingSystem> _shopping_system;
    std::shared_ptr<Viewport> _viewport;
    std::shared_ptr<LevelSummaryTracker> _level_summary_tracker;
    std::function<bool(const FrameStats&)> _loop;
};
