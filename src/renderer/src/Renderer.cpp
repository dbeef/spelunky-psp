#include "Renderer.hpp"
#include "LevelGenerator.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "video/Context.hpp"
#include "glad/glad.h"
#include "logger/log.h"
#include "cJSON.h"

#include "graphics_utils/LookAt.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "graphics_utils/DebugGlCall.hpp"

#include <string>
#include <cstring>

Renderer *Renderer::_level_renderer = nullptr;

Renderer &Renderer::instance()
{
    assert(_level_renderer);
    return *_level_renderer;
}

void Renderer::init()
{
    assert(!_level_renderer);
    _level_renderer = new Renderer();
}

void Renderer::dispose()
{
    assert(_level_renderer);
    delete _level_renderer;
    _level_renderer = nullptr;
}

void Renderer::render() const
{
    for (const auto& entity : _render_entities)
    {
        // Interleaving vertex attributes instead of separate buffers for small performance boost from data locality:
        const auto *vertices = reinterpret_cast<const char *>(entity.vertices);
        const auto *uvs = vertices + 2 * sizeof(float);

        // Tightly packed Vertex type:
        const size_t stride = sizeof(Vertex);

        DebugGlCall(glBindTexture(GL_TEXTURE_2D, entity.texture));

        DebugGlCall(glVertexPointer(2, GL_FLOAT, stride, vertices));
        DebugGlCall(glTexCoordPointer(2, GL_FLOAT, stride, uvs));

        DebugGlCall(glDrawElements(GL_TRIANGLES, entity.indices_count, GL_UNSIGNED_SHORT, entity.indices));
    }
}

void Renderer::update()
{
    for (const auto &id : _for_removal)
    {
        const auto predicate = [&id](const RenderEntity& e) { return e.id == id;};
        const auto it = std::remove_if(_render_entities.begin(), _render_entities.end(), predicate);
        if (it != _render_entities.end())
        {
            _render_entities.erase(it);
        }
    }
}
