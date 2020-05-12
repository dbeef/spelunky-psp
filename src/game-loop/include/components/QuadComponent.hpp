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

    template <class T>
    void frame_changed(T frame_index) { _frame_index = static_cast<std::size_t>(frame_index);_frame_changed = true; }

    template <class T>
    T get_current_frame() const { return static_cast<T>(_frame_index); }

private:

    std::size_t _frame_index = 0;
    bool _frame_changed = true;

    Quad _quad;
    RenderEntityID _render_entity_id;
    const TextureType _texture_type;
};
