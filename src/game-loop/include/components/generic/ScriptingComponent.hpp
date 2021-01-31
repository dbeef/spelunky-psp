#pragma once

#include <memory>
#include <cstdint>

#include "entt/entt.hpp"

class ScriptBase
{
public:
    virtual void update(entt::entity owner, uint32_t delta_time_ms) = 0;
};

struct ScriptingComponent
{
    explicit ScriptingComponent(std::shared_ptr<ScriptBase> script) : script(std::move(script)) {}
    ScriptingComponent() = default;

    std::shared_ptr<ScriptBase> script = nullptr;
};
