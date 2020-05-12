#pragma once

#include <cstdint>

#include "TextureType.hpp"
#include "Quad.hpp"
#include "MapTile.hpp"

class MainDude;

class QuadComponent
{
public:

    explicit QuadComponent(TextureType type);
    ~QuadComponent();
    void update(MainDude&, uint32_t delta_time_ms);

private:

    Quad _quad;
    RenderEntityID _render_entity_id;
    const TextureType _texture_type;
};
