#pragma once

#include "entt/entt.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "patterns/Subject.hpp"
#include "other/Inventory.hpp"

namespace prefabs
{
    struct BombSpawner
    {
        class BombSpawnerScript final : public ScriptBase, public Subject<BombCountChangedEvent>
        {
        public:
            explicit BombSpawnerScript(int bomb_counter) : _bomb_counter(bomb_counter) {}
            void update(entt::entity owner, uint32_t delta_time_ms) override;
            void remove_bombs(int amount) { _bomb_counter -= amount; notify({_bomb_counter}); }
            void add_bombs(int amount) { _bomb_counter += amount; notify({_bomb_counter}); }
            int get_bombs() const { return _bomb_counter; }
        private:
            int _bomb_counter = 0;
        };

        static entt::entity create(int initial_bomb_count);
    };
}
