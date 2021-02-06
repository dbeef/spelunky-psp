#include <components/generic/QuadComponent.hpp>
#include <spritesheet-frames/NPCSpritesheetFrames.hpp>
#include <components/generic/MeshComponent.hpp>
#include "prefabs/traps/ArrowTrap.hpp"

#include "components/generic/HorizontalOrientationComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/ScriptingComponent.hpp"

#include "EntityRegistry.hpp"

namespace
{
    class ArrowTrapScript : public ScriptBase
    {
    public:
        explicit ArrowTrapScript(HorizontalOrientation orientation) : _orientation(orientation) {}

        void update(entt::entity owner, uint32_t delta_time_ms) override
        {
            auto& registry = EntityRegistry::instance().get_registry();
        }

    private:
        const HorizontalOrientation _orientation;
    };
}

entt::entity prefabs::ArrowTrap::create(float pos_x_center, float pos_y_center, HorizontalOrientation orientation)
{
    auto& registry = EntityRegistry::instance().get_registry();

    const auto entity = registry.create();

    registry.emplace<PositionComponent>(entity, pos_x_center, pos_y_center);
    registry.emplace<ScriptingComponent>(entity, std::make_shared<ArrowTrapScript>(orientation));

    // Temporarily:

    QuadComponent quad(TextureType::NPC, 1.0f, 1.0f);
    MeshComponent mesh;
    quad.frame_changed(NPCSpritesheetFrames::SPIDER_HANG);
    mesh.rendering_layer = RenderingLayer::LAYER_2_ITEMS;
    mesh.camera_type = CameraType::MODEL_VIEW_SPACE;

    registry.emplace<QuadComponent>(entity, quad);
    registry.emplace<MeshComponent>(entity, mesh);

    return entity;
}
