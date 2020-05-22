#include <game-objects/MainLogo.hpp>
#include "main-dude/MainDude.hpp"
#include "Renderer.hpp"
#include "TextureBank.hpp"
#include "components/QuadComponent.hpp"
#include "game-objects/MainLogo.hpp"

void QuadComponent::update(MainDude &main_dude, uint32_t delta_time_ms)
{
    if (_frame_changed)
    {
        const auto texture_region = TextureBank::instance().get_region(_texture_type, _frame_index);
        const bool vflip = !main_dude._other.facing_left;

        texture_region.set_quad_xy(_quad);
        texture_region.set_quad_uv(_quad, vflip);
        texture_region.set_quad_indices(_quad);

        _frame_changed = false;
    }

    // Make quad center to be at 0.0:
    const float pos_x = main_dude._physics.get_x_position() - (_quad_dimensions.width / 2);
    const float pos_y = main_dude._physics.get_y_position() - (_quad_dimensions.height / 2);

    _quad.set_translation(pos_x, pos_y);
    _quad.set_scale(_quad_dimensions.width, _quad_dimensions.height);
    _quad.write();
}

QuadComponent::QuadComponent(TextureType type, float quad_width, float quad_height)
    : _quad_dimensions {quad_width, quad_height}
    , _texture_type(type)
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

void QuadComponent::update(MainLogo& main_logo, uint32_t delta_time_ms)
{
    // FIXME: Probably this should be common between game objects, as nothing differs between MainDude and MainLogo.

    if (_frame_changed)
    {
        const auto texture_region = TextureBank::instance().get_region(_texture_type, _frame_index);

        texture_region.set_quad_xy(_quad);
        texture_region.set_quad_uv(_quad);
        texture_region.set_quad_indices(_quad);

        _frame_changed = false;
    }

    // Make quad center to be at 0.0:
    const float pos_x = main_logo._position.x_center - (_quad_dimensions.width / 2);
    const float pos_y = main_logo._position.y_center - (_quad_dimensions.height / 2);

    _quad.set_translation(pos_x, pos_y);
    _quad.set_scale(_quad_dimensions.width, _quad_dimensions.height);
    _quad.write();
}
