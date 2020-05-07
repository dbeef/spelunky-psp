#pragma once

#include "glad/glad.h"
#include "MapTileType.hpp"
#include "RenderEntity.hpp"

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

    RenderEntityID add_entity(RenderEntity e)
    {
        static RenderEntityID unique_id_pool = std::numeric_limits<RenderEntityID>::max();
        unique_id_pool--;

        e.id = unique_id_pool;
        _render_entities.emplace_back(e);
        return unique_id_pool;
    }

    RenderEntity* get_entity(const RenderEntityID id)
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

    void mark_for_removal(const RenderEntityID id)
    {
        _for_removal.push_back(id);
    }

private:

    std::vector<RenderEntity> _render_entities;
    std::vector<RenderEntityID> _for_removal;

    static Renderer* _level_renderer;
};
