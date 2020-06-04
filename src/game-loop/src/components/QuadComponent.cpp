#include <components/QuadComponent.hpp>
#include "main-dude/MainDude.hpp"
#include "Renderer.hpp"
#include "TextureBank.hpp"
#include "components/QuadComponent.hpp"

QuadComponent::QuadComponent(TextureType texture_type, Renderer::EntityType entity_type, float quad_width, float quad_height)
    : _quad_dimensions {quad_width, quad_height}
    , _texture_type(texture_type)
    , _entity_type(entity_type)
{
    _render_entity_id = Renderer::instance().add_entity(_quad, TextureBank::instance().get_texture(_texture_type), _entity_type);
    assert(_render_entity_id != Renderer::INVALID_ENTITY);
}

QuadComponent::QuadComponent(const QuadComponent& other)
        : _quad_dimensions {other._quad_dimensions.width, other._quad_dimensions.height}
        , _texture_type(other._texture_type)
        , _entity_type(other._entity_type)
{
    _render_entity_id = Renderer::instance().add_entity(_quad, TextureBank::instance().get_texture(_texture_type), _entity_type);
    assert(_render_entity_id != Renderer::INVALID_ENTITY);
}

QuadComponent::~QuadComponent()
{
    if (_render_entity_id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity_id, _entity_type);
    }
}

void QuadComponent::update(float pos_x_center, float pos_y_center, bool vertical_flip)
{
    if (_frame_changed)
    {
        const auto texture_region = TextureBank::instance().get_region(_texture_type, _frame_index);

        texture_region.set_quad_xy(_quad);
        texture_region.set_quad_uv(_quad, vertical_flip);
        texture_region.set_quad_indices(_quad);

        _frame_changed = false;
    }

    // Make quad center to be at 0.0:
    const float pos_x = pos_x_center - (_quad_dimensions.width / 2);
    const float pos_y = pos_y_center - (_quad_dimensions.height / 2);

    _quad.set_translation(pos_x, pos_y);
    _quad.set_scale(_quad_dimensions.width, _quad_dimensions.height);
    _quad.write();
}
