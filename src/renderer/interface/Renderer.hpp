#pragma once

#include "MapTileType.hpp"
#include "RenderEntity.hpp"
#include "Mesh.hpp"

#include <limits>
#include <algorithm>
#include <vector>

class Renderer
{
public:

    enum class EntityType
    {
        SCREEN_SPACE = 0,
        MODEL_VIEW_SPACE,
        _SIZE
    };

    static const RenderEntityID INVALID_ENTITY = 0;

    static Renderer& instance();
    static void init();
    static void dispose();

    void render(EntityType type) const;
    void update();

    RenderEntityID add_entity(Mesh& mesh, TextureID texture, EntityType type)
    {
        RenderEntity entity;
        entity.indices_count = mesh.indices.size();
        entity.indices = mesh.indices.data();
        entity.vertices = mesh.vertices.data();
        entity.texture = texture;
        return add_entity(entity, type);
    }

    RenderEntityID add_entity(Quad& quad, TextureID texture, EntityType type)
    {
        RenderEntity entity;
        entity.indices_count = Quad::get_indices_count();
        entity.indices = quad.get_indices();
        entity.vertices = quad.get_vertices_transformed();
        entity.texture = texture;
        return add_entity(entity, type);
    }

    RenderEntityID add_entity(RenderEntity e, EntityType type)
    {
        static RenderEntityID unique_id_pool = std::numeric_limits<RenderEntityID>::max();

        e.id = --unique_id_pool;
        _render_entities[static_cast<std::size_t>(type)].emplace_back(e);
        return unique_id_pool;
    }

    void mark_for_removal(RenderEntityID id, EntityType type)
    {
        _for_removal[static_cast<std::size_t>(type)].push_back(id);
    }

private:

    std::vector<RenderEntity> _render_entities[static_cast<std::size_t>(EntityType::_SIZE)];
    std::vector<RenderEntityID> _for_removal[static_cast<std::size_t>(EntityType::_SIZE)];

    static Renderer* _renderer;
};
