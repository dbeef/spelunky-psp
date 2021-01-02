#include "EntityRegistry.hpp"
#include "prefabs/main-dude/MainDude.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/MainMenuSpritesheetFrames.hpp"

namespace prefabs
{
    entt::entity MainDude::create()
    {
        return create(0, 0);
    }

    entt::entity MainDude::create(float pos_x_center, float pos_y_center)
    {
        auto& registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();

        PhysicsComponent physics(1 - (2.0f/16.0f), 1 - (1.0f / 16.0f));

        PositionComponent position(pos_x_center, pos_y_center);
        QuadComponent quad(TextureType::MAIN_DUDE, 1, 1);
        AnimationComponent animation;
        MeshComponent mesh;

        mesh.camera_type = CameraType::MODEL_VIEW_SPACE;
        mesh.rendering_layer = RenderingLayer::LAYER_3_DUDE;

        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<QuadComponent>(entity, quad);
        registry.emplace<AnimationComponent>(entity, animation);
        registry.emplace<PhysicsComponent>(entity, physics);
        registry.emplace<MeshComponent>(entity, mesh);

        // Initialization order is important in this case - MainDudeComponent must be the last to create.
        MainDudeComponent main_dude(entity);
        registry.emplace<MainDudeComponent>(entity, main_dude);

        return entity;
    }
}
