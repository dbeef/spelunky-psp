#include "components/generic/TextComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "spritesheet-frames/FontSpritesheetFrames.hpp"
#include "EntityRegistry.hpp"

void TextComponent::update(entt::entity owner)
{
    if (!_dirty)
    {
        return;
    }

    auto& registry = EntityRegistry::instance().get_registry();

    _mesh.clear();
    _indices.clear();

    auto& mesh_component = registry.get<MeshComponent>(owner);
    auto& position_component = registry.get<PositionComponent>(owner);

    for (std::size_t index = 0; index < _properties.text.size(); index++)
    {
        char sign = _properties.text[index];

        if (sign == 0)
        {
            break;
        }

        float x_center = position_component.x_center + (index * get_font_width());
        float y_center = position_component.y_center;

        const int16_t spritesheet_frame_index = sign + (_properties.yellow ? FONT_ASCII_YELLOW_OFFSET : FONT_ASCII_OFFSET);
        const auto &texture_region = TextureBank::instance().get_region(TextureType::FONT, spritesheet_frame_index);

        auto mesh = texture_region.get_quad_vertices(0, 0);

        for(auto& vertex : mesh)
        {
            vertex.x *= get_font_width();
            vertex.y *= get_font_height();

            vertex.x += x_center;
            vertex.y += y_center;
        }

        const auto indices = texture_region.get_quad_indices(index);

        std::copy(mesh.begin(), mesh.end(), std::back_inserter(_mesh));
        std::copy(indices.begin(), indices.end(), std::back_inserter(_indices));
    }

    mesh_component.vertices = _mesh.data();
    mesh_component.indices = _indices.data();
    mesh_component.indices_count = _indices.size();
    mesh_component.texture_id = TextureBank::instance().get_texture(TextureType::FONT);
    mesh_component.rendering_layer = _properties.layer;
    mesh_component.camera_type = CameraType::SCREEN_SPACE;

    _dirty = false;
}
