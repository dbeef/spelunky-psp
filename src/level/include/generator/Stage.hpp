#pragma once
#include "LevelType.hpp"

class TileBatch;
class EntityRegistry;

namespace Stage {

    enum class Type {
        GENERATE_INDESTRUCTIBLE_FRAME = 0,
        GENERATE_BACKGROUND,
        GENERATE_LOOT,
        GENERATE_NPC,
        _SIZE
    };

    template<Type>
    void run(TileBatch &output_batch, EntityRegistry &output_registry, LevelType);

    template<>
    void run<Type::GENERATE_BACKGROUND>(TileBatch &output_batch, EntityRegistry&, LevelType);

    template<>
    void run<Type::GENERATE_INDESTRUCTIBLE_FRAME>(TileBatch &output_batch, EntityRegistry&, LevelType);

    template<>
    void run<Type::GENERATE_LOOT>(TileBatch &output_batch, EntityRegistry&, LevelType);

    template<>
    void run<Type::GENERATE_NPC>(TileBatch &output_batch, EntityRegistry&, LevelType);
}
