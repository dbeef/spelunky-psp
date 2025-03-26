#pragma once

#include "entt/entt.hpp"
#include "viewport/Viewport.hpp"
#include "MapTileType.hpp"

#include <vector>
#include <functional>

namespace prefabs
{
    class TileBrowserWindowComponent
    {
    public:
        MapTileType get_selected_tile_type() const { return _selected_tile_type; }
        void set_selected_tile_type(MapTileType selected_tile_type) { _selected_tile_type = selected_tile_type;}
    private:
        MapTileType _selected_tile_type{};
    };

    struct TileBrowserWindow
    {
        static entt::entity create(const std::shared_ptr<Viewport>& viewport);
    };
}
