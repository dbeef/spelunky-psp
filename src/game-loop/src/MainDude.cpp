//
// Created by dbeef on 5/7/20.
//

#include <MainDude.hpp>
#include <MapTileType.hpp>
#include <Renderer.hpp>

MainDude::MainDude()
{
    const auto texture_region = TextureBank::instance().get_region(TextureType::CAVE_LEVEL_TILES, static_cast<std::size_t>(MapTileType::STONE_BLOCK));

    int pos_x = 0;
    int pos_y = 0;

    _mesh = texture_region.get_quad_mesh(pos_x, pos_y);
    _indices = texture_region.get_quad_indices();

    // Render entity

    _render_entity.indices_count = _indices.size();
    _render_entity.indices = _indices.data();
    _render_entity.vertices = _mesh.data();
    _render_entity.texture = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);

    _render_entity.id = Renderer::instance().add_entity(_render_entity);
}

MainDude::~MainDude()
{
    if (_render_entity.id != Renderer::INVALID_ENTITY)
    {
        Renderer::instance().mark_for_removal(_render_entity.id);
    }
}

