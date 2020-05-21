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

    static const RenderEntityID INVALID_ENTITY = 0;

    static Renderer& instance();
    static void init();
    static void dispose();

    void render() const;
    void update();

    RenderEntityID add_entity(Mesh& mesh, TextureID texture)
    {
        RenderEntity entity;
        entity.indices_count = mesh.indices.size();
        entity.indices = mesh.indices.data();
        entity.vertices = mesh.vertices.data();
        entity.texture = texture;
        return add_entity(entity);
    }

    RenderEntityID add_entity(RenderEntity e)
    {
        static RenderEntityID unique_id_pool = std::numeric_limits<RenderEntityID>::max();

        e.id = --unique_id_pool;
        _render_entities.emplace_back(e);
        return unique_id_pool;
    }

    RenderEntity* get_entity(RenderEntityID id)
    {
        const auto it = std::find_if(_render_entities.begin(), _render_entities.end(), [&id](const RenderEntity& e)
        {
            return e.id == id;
        });

        if (it != _render_entities.end())
        {
            return &*it;
        }

        return nullptr;
    }

    void mark_for_removal(RenderEntityID id)
    {
        _for_removal.push_back(id);
    }

private:

    std::vector<RenderEntity> _render_entities;
    std::vector<RenderEntityID> _for_removal;

    static Renderer* _level_renderer;
};
