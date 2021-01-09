#include "CameraType.hpp"
#include "TextureBank.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/HorizontalOrientationComponent.hpp"
#include "EntityRegistry.hpp"

QuadComponent::QuadComponent(TextureType texture_type, float quad_width, float quad_height)
        : _quad_dimensions {quad_width, quad_height}
        , _texture_type(texture_type)
{
}

void QuadComponent::update(entt::entity owner)
{
    auto& registry = EntityRegistry::instance().get_registry();

    if (_frame_changed)
    {
        const auto texture_region = TextureBank::instance().get_region(_texture_type, _frame_index);

        texture_region.set_quad_xy(_quad);
        texture_region.set_quad_uv(_quad, _vertical_flip);
        texture_region.set_quad_indices(_quad);

        _frame_changed = false;
    }

    auto& position = registry.get<PositionComponent>(owner);

    _quad.set_translation(position.x_center, position.y_center);
    _quad.set_scale(_quad_dimensions.width, _quad_dimensions.height);
    _quad.write();

    auto& mesh = registry.get<MeshComponent>(owner);

    mesh.vertices = _quad.get_vertices_transformed();
    mesh.indices = _quad.get_indices();
    mesh.indices_count = _quad.get_indices_count();
    mesh.texture_id = TextureBank::instance().get_texture(_texture_type);

    if (registry.has<HorizontalOrientationComponent>(owner))
    {
        auto& orientation = registry.get<HorizontalOrientationComponent>(owner);

        // All spritesheet frames are by default left-oriented:
        set_vertical_flip(orientation.orientation == HorizontalOrientation::RIGHT);
        frame_changed();
    }
}
