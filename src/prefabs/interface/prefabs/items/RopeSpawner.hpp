#pragma once

#include "entt/entt.hpp"
#include "other/Inventory.hpp"
#include "components/generic/ScriptingComponent.hpp"

namespace prefabs
{
    struct RopeSpawner
    {
        class RopeSpawnerScript final : public ScriptBase, public Subject<RopeCountChangedEvent>
        {
        public:
            explicit RopeSpawnerScript(int Rope_counter) : _rope_counter(Rope_counter) {}
            void update(entt::entity owner, uint32_t delta_time_ms) override;
            void remove_ropes(int amount) { _rope_counter -= amount; notify({_rope_counter}); }
            void add_ropes(int amount) { _rope_counter += amount; notify({_rope_counter}); }
            int get_ropes() const { return _rope_counter; }
        private:
            int _rope_counter = 0;
        };

        static entt::entity create(int initial_rope_count);
    };
}
