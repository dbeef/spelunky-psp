#include "main-dude/MainDude.hpp"
#include "Renderer.hpp"
#include "TextureBank.hpp"
#include "components/QuadComponent.hpp"

void QuadComponent::update(MainDude& main_dude, uint32_t delta_time_ms)
{
    const auto current_frame_index = static_cast<std::size_t>(main_dude._current_frame);
    const auto texture_region = TextureBank::instance().get_region(TextureType::MAIN_DUDE, current_frame_index);
    const bool vflip = !main_dude._facing_left;

    texture_region.set_quad_vertices(_quad, vflip);
    texture_region.set_quad_indices(_quad);

    // Make quad center to be at 0.0:
    float pos_x = main_dude._x - main_dude._physics_component.get_width() / 2;
    float pos_y = main_dude._y - main_dude._physics_component.get_height() / 2;

    _quad.translate(pos_x, pos_y);
    _quad.scale(main_dude._physics_component.get_width(), main_dude._physics_component.get_height());
}

QuadComponent::QuadComponent(TextureType type) : _texture_type(type)
{
    RenderEntity entity;
    entity.vertices = _quad.get_vertices();
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
