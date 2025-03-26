#include "generator/Pipeline.hpp"

#include "SplashScreenType.hpp"
#include "generator/Stage.hpp"
#include "TileBatch.hpp"

namespace Pipeline {
    template<>
    void run<LevelType::CAVE>(TileBatch &output_batch, EntityRegistry &entity_registry) {

        RoomLayoutGenerator room_layout_generator;
        room_layout_generator.generate({});

        output_batch.resize(
            room_layout_generator.get_width_tiles() + 2,
            room_layout_generator.get_height_tiles() + 2
        );
        output_batch.initialise_tiles_from_room_layout(room_layout_generator);

        Stage::run<Stage::Type::GENERATE_BACKGROUND>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_INDESTRUCTIBLE_FRAME>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_LOOT>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_NPC>(output_batch, entity_registry, LevelType::CAVE);

        output_batch.batch_vertices();
    }

    template<>
    void run<LevelType::MAIN_MENU>(TileBatch &output_batch, EntityRegistry &entity_registry) {
        output_batch.initialise_tiles_from_splash_screen(SplashScreenType::MAIN_MENU);

        Stage::run<Stage::Type::GENERATE_BACKGROUND>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_INDESTRUCTIBLE_FRAME>(output_batch, entity_registry, LevelType::CAVE);

        output_batch.batch_vertices();
    }

    template<>
    void run<LevelType::LEVEL_SUMMARY>(TileBatch &output_batch, EntityRegistry &entity_registry) {
        output_batch.initialise_tiles_from_splash_screen(SplashScreenType::LEVEL_SUMMARY);

        Stage::run<Stage::Type::GENERATE_BACKGROUND>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_INDESTRUCTIBLE_FRAME>(output_batch, entity_registry, LevelType::CAVE);

        output_batch.batch_vertices();
    }

    template<>
    void run<LevelType::SCORES>(TileBatch &output_batch, EntityRegistry &entity_registry) {
        output_batch.initialise_tiles_from_splash_screen(SplashScreenType::SCORES);

        Stage::run<Stage::Type::GENERATE_BACKGROUND>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_INDESTRUCTIBLE_FRAME>(output_batch, entity_registry, LevelType::CAVE);

        output_batch.batch_vertices();
    }

    template<>
    void run<LevelType::SANDBOX>(TileBatch &output_batch, EntityRegistry &entity_registry) {
        output_batch.clean();

        output_batch.initialise_tiles_from_splash_screen(SplashScreenType::SANDBOX);
        Stage::run<Stage::Type::GENERATE_BACKGROUND>(output_batch, entity_registry, LevelType::CAVE);
        Stage::run<Stage::Type::GENERATE_INDESTRUCTIBLE_FRAME>(output_batch, entity_registry, LevelType::CAVE);

        output_batch.batch_vertices();
    }
}
