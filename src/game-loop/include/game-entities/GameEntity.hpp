#pragma once

#include <utility>
#include <cstdint>
#include <memory>

#include "components/PhysicsComponent.hpp"
#include "components/QuadComponent.hpp"
#include "components/TextComponent.hpp"
#include "components/AnimationComponent.hpp"

#include "logger/log.h"

class World;

class GameEntity
{
public:

    enum class Type
    {
        MAIN_DUDE,
        JAR,
        BIG_GEM,
        CHEST,
        COPYRIGHTS_SIGN,
        LOGO,
        QUIT_SIGN,
        RESET_SIGN,
        DEATH_OVERLAY,
        PAUSE_OVERLAY,
        HUD,
        LEVEL_SUMMARY_OVERLAY,
        LEVEL_SUMMARY_TRACKER,
        ROCK,
        SCORES_SIGN,
        SINGLE_GOLD_BAR,
        START_SIGN,
        TRIPLE_GOLD_BAR,
        TUTORIAL_SIGN,
        SCORES_OVERLAY,
    };

    const char* type_to_string(Type type)
    {
#define TO_STR(x) case x: return #x;
        switch (type)
        {
            TO_STR(Type::MAIN_DUDE)
            TO_STR(Type::JAR)
            TO_STR(Type::BIG_GEM)
            TO_STR(Type::CHEST)
            TO_STR(Type::COPYRIGHTS_SIGN)
            TO_STR(Type::LOGO)
            TO_STR(Type::QUIT_SIGN)
            TO_STR(Type::RESET_SIGN)
            TO_STR(Type::DEATH_OVERLAY)
            TO_STR(Type::PAUSE_OVERLAY)
            TO_STR(Type::HUD)
            TO_STR(Type::LEVEL_SUMMARY_OVERLAY)
            TO_STR(Type::LEVEL_SUMMARY_TRACKER)
            TO_STR(Type::ROCK)
            TO_STR(Type::SCORES_SIGN)
            TO_STR(Type::SINGLE_GOLD_BAR)
            TO_STR(Type::START_SIGN)
            TO_STR(Type::TRIPLE_GOLD_BAR)
            TO_STR(Type::TUTORIAL_SIGN)
            TO_STR(Type::SCORES_OVERLAY)
            default: assert(false);
        }
#undef TO_STR
    }

    virtual ~GameEntity()
    {
        log_debug("Disposing entity: %s", type_to_string(_type));
    }

    virtual void update(World* world, uint32_t delta_time_ms) = 0;
    bool is_marked_for_disposal() const { return _to_dispose; }

    explicit GameEntity(Type type) : _type(type)
    {
        log_debug("Constructing entity: %s", type_to_string(_type));
    };

    PhysicsComponent* get_physics_component() const { return _physics_component.get(); }
    QuadComponent* get_quad_component() const { return _quad_component.get(); }
    AnimationComponent* get_animation_component() const { return _animation_component.get(); }
    TextComponent* get_text_component() const { return _text_component.get(); }

protected:
    void mark_for_disposal() { _to_dispose = true; }

    std::shared_ptr<PhysicsComponent> _physics_component = nullptr;
    std::shared_ptr<QuadComponent> _quad_component = nullptr;
    std::shared_ptr<AnimationComponent> _animation_component = nullptr;
    std::shared_ptr<TextComponent> _text_component = nullptr;

    const Type _type;
private:
    bool _to_dispose = false;
};
