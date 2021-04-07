#include "prefabs/main-dude/MainDude.hpp"
#include "prefabs/items/BombSpawner.hpp"
#include "prefabs/items/RopeSpawner.hpp"
#include "prefabs/items/Whip.hpp"

#include "components/damage/GiveJumpOnTopDamage.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ClimbingComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/ItemCarrierComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/InputComponent.hpp"
#include "components/specialized/MainDudeComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"

namespace prefabs
{
    entt::entity MainDude::create()
    {
        return create(0, 0);
    }

    entt::entity MainDude::create(float pos_x_center, float pos_y_center) {
        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();

        PhysicsComponent physics(1 - (2.0f / 16.0f), 1 - (1.0f / 16.0f));

        PositionComponent position(pos_x_center, pos_y_center);
        QuadComponent quad(TextureType::MAIN_DUDE, 1, 1);
        AnimationComponent animation;
        InputComponent input;
        ItemCarrierComponent item_carrier;

        MeshComponent mesh;
        mesh.camera_type = CameraType::MODEL_VIEW_SPACE;
        mesh.rendering_layer = RenderingLayer::LAYER_3_DUDE;

        registry.emplace<PositionComponent>(entity, position);
        registry.emplace<QuadComponent>(entity, quad);
        registry.emplace<AnimationComponent>(entity, animation);
        registry.emplace<PhysicsComponent>(entity, physics);
        registry.emplace<MeshComponent>(entity, mesh);
        registry.emplace<InputComponent>(entity, input);
        registry.emplace<HorizontalOrientationComponent>(entity);
        registry.emplace<ItemCarrierComponent>(entity, item_carrier);
        registry.emplace<GiveJumpOnTopDamageComponent>(entity, 1);

        // Initialization order is important in this case - MainDudeComponent must be the last to create.
        MainDudeComponent main_dude(entity);
        registry.emplace<MainDudeComponent>(entity, main_dude);

        ClimbingComponent climbing;
        climbing.add_observer(reinterpret_cast<Observer<ClimbingEvent> *>(main_dude.get_climbing_observer()));
        registry.emplace<ClimbingComponent>(entity, climbing);

        {
            auto& carrier = registry.get<ItemCarrierComponent>(entity);

            carrier.pick_up_item(prefabs::BombSpawner::create(), entity);
            carrier.pick_up_item(prefabs::RopeSpawner::create(), entity);
            carrier.pick_up_item(prefabs::Whip::create(pos_x_center, pos_y_center), entity);
        }

        return entity;
    }
}
