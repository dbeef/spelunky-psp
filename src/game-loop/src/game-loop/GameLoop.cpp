#include "game-loop/GameLoop.hpp"

#include "viewport/Viewport.hpp"
#include "components/specialized/LevelSummaryTracker.hpp"
#include "system/RenderingSystem.hpp"
#include "system/DisposingSystem.hpp"
#include "system/InputSystem.hpp"
#include "system/ScriptingSystem.hpp"
#include "system/CollectibleSystem.hpp"
#include "system/PhysicsSystem.hpp"
#include "system/ParticleSystem.hpp"
#include "system/AnimationSystem.hpp"
#include "system/ItemSystem.hpp"
#include "system/DamageSystem.hpp"

#include <algorithm>
#include <cassert>

std::function<bool(uint32_t delta_time_ms)>& GameLoop::get()
{
    return _loop;
}

GameLoop::GameLoop(const std::shared_ptr<Viewport>& viewport)
    : _viewport(viewport)
    , _level_summary_tracker(std::make_shared<LevelSummaryTracker>())
    , _rendering_system(std::make_shared<RenderingSystem>(viewport))
    , _scripting_system(std::make_shared<ScriptingSystem>())
    , _physics_system(std::make_shared<PhysicsSystem>())
    , _animation_system(std::make_shared<AnimationSystem>())
    , _collectible_system(std::make_shared<CollectibleSystem>())
    , _input_system(std::make_shared<InputSystem>())
    , _disposing_system(std::make_shared<DisposingSystem>())
    , _particle_system(std::make_shared<ParticleSystem>())
    , _item_system(std::make_shared<ItemSystem>())
    , _damage_system(std::make_shared<DamageSystem>())
{
    _states.current = &_states.started;
    _states.current->enter(*this);

    _loop = [this](uint32_t delta_time_ms)
    {
        assert(_states.current);
        auto new_state = _states.current->update(*this, delta_time_ms);

        if (new_state != _states.current)
        {
            _states.current->exit(*this);
            new_state->enter(*this);
            _states.current = new_state;
        }

        return _exit;
    };
}
