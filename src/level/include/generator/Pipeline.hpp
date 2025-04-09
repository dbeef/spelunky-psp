#pragma once

#include "LevelType.hpp"

class TileBatch;
class EntityRegistry;

namespace Pipeline {
    template<LevelType>
    void run(TileBatch &output_batch, EntityRegistry &output_registry);

    template<>
    void run<LevelType::CAVE>(TileBatch &output_batch, EntityRegistry&);

    template<>
    void run<LevelType::MAIN_MENU>(TileBatch &output_batch, EntityRegistry&);

    template<>
    void run<LevelType::LEVEL_SUMMARY>(TileBatch &output_batch, EntityRegistry&);

    template<>
    void run<LevelType::SCORES>(TileBatch &output_batch, EntityRegistry&);

    template<>
    void run<LevelType::SANDBOX>(TileBatch &output_batch, EntityRegistry&);
}
