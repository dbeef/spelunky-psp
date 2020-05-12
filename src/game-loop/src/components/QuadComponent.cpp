#include "main-dude/MainDude.hpp"
#include "Renderer.hpp"
#include "TextureBank.hpp"
#include "components/QuadComponent.hpp"

void QuadComponent::update(MainDude &main_dude, uint32_t delta_time_ms)
{
    if (_frame_changed)
    {
        const auto texture_region = TextureBank::instance().get_region(_texture_type, _frame_index);
        const bool vflip = !main_dude._facing_left;

        texture_region.set_quad_xy(_quad);
        texture_region.set_quad_uv(_quad, vflip);
        texture_region.set_quad_indices(_quad);

        _frame_changed = false;
    }

    // Make quad center to be at 0.0:
    const float pos_x = main_dude._x - main_dude._physics_component.get_width() / 2;
    const float pos_y = main_dude._y - main_dude._physics_component.get_height() / 2;

    _quad.set_translation(pos_x, pos_y);
    _quad.set_scale(main_dude._physics_component.get_width(), main_dude._physics_component.get_height());
    _quad.write();
}

QuadComponent::QuadComponent(TextureType type) : _texture_type(type)
{
    RenderEntity entity;
    entity.vertices = _quad.get_vertices_transformed();
    entity.indices = _quad.get_indices();
    entity.indices_count = Quad::get_indices_count();
    entity.texture = TextureBank::instance().get_texture(_texture_type);
    _render_entity_id = Renderer::instance().add_entity(entity);
    assert(_render_entity_id != Renderer::INVALID_ENTITY);
}

QuadComponent::~QuadComponent()
{
    if (_render_entity_id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity_id);
    }
}
