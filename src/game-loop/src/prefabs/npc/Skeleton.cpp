 #include "prefabs/npc/Skeleton.hpp"

#include "components/specialized/MainDudeComponent.hpp"
#include "components/generic/AnimationComponent.hpp"
#include "components/generic/PhysicsComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"
#include "TextureType.hpp"
#include "spritesheet-frames/NPCSpritesheetFrames.hpp"

#include <cmath>

namespace
{
    constexpr float activation_distance_x = 3.0f;
    constexpr float activation_distance_y = 0.5f;

    class SkeletonScript final : public ScriptBase
    {
    public:
        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
            const auto& skeleton_position = registry.get<PositionComponent>(owner);

            if (!_started_transformation)
            {
                const auto main_dudes = registry.view<MainDudeComponent, PositionComponent>();
                main_dudes.each([&](MainDudeComponent&, PositionComponent& dude_position)
                {
                    if (_started_transformation)
                    {
                        return;
                    }

                    float distance_x = std::fabs(dude_position.x_center - skeleton_position.x_center);
                    float distance_y = std::fabs(dude_position.y_center - skeleton_position.y_center);

                    if (distance_x < activation_distance_x && distance_y < activation_distance_y)
                    {
                        _started_transformation = true;

                        if (dude_position.x_center > skeleton_position.x_center)
                        {
                            _orientation = HorizontalOrientation::RIGHT;
                        }
                        else
                        {
                            _orientation = HorizontalOrientation::LEFT;
                        }

                        auto& animation = registry.get<AnimationComponent>(owner);
                        animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::SKELETON_CREATE_0_FIRST),
                                        static_cast<std::size_t>(NPCSpritesheetFrames::SKELETON_CREATE_5_LAST),
                                        100, false);
                    }
                });
            }
            else if (!_finished_transformation)
            {
                auto& animation = registry.get<AnimationComponent>(owner);
                if (animation.is_finished())
                {
                    animation.start(static_cast<std::size_t>(NPCSpritesheetFrames::SKELETON_WALK_0_FIRST),
                                    static_cast<std::size_t>(NPCSpritesheetFrames::SKELETON_WALK_4_LAST),
                                    100, true);
                    _finished_transformation = true;
                }
            }
            else
            {
                _walking_timer += delta_time_ms;
                auto& physics = registry.get<PhysicsComponent>(owner);

                if (physics.is_left_collision())
                {
                    _orientation = HorizontalOrientation::RIGHT;
                }
                else if (physics.is_right_collision())
                {
                    _orientation = HorizontalOrientation::LEFT;
                }

                if (_walking_timer > 20)
                {
                    switch (_orientation)
                    {
                        case HorizontalOrientation::LEFT: physics.set_x_velocity(-0.018f); break;
                        case HorizontalOrientation::RIGHT: physics.set_x_velocity(0.018f); break;
                    }

                    _walking_timer = 0;
                }
            }
        }
    private:
        bool _started_transformation = false;
        bool _finished_transformation = false;
        HorizontalOrientation _orientation;
        uint32_t _walking_timer;
    };
}

entt::entity prefabs::Skeleton::create()
{
    return create(0, 0);
}

entt::entity prefabs::Skeleton::create(float pos_x_center, float pos_y_center)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    const float width = 1.0f;
    const float height = 1.0f;

    QuadComponent quad(TextureType::NPC, width, height);
    quad.frame_changed(NPCSpritesheetFrames::SKELETON_CREATE_0_FIRST);

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<AnimationComponent>(entity);
    registry.emplace<MeshComponent>(entity, RenderingLayer::LAYER_2_ITEMS, CameraType::MODEL_VIEW_SPACE);
    registry.emplace<PhysicsComponent>(entity, width - (4.0f / 16.0f), height - (2.0f / 16.0f));
    registry.emplace<ScriptingComponent>(entity, std::make_shared<SkeletonScript>());
    registry.emplace<HorizontalOrientationComponent>(entity);

    return entity;
}
