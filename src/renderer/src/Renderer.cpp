#include "Renderer.hpp"
#include "TextureBank.hpp"
#include "TextureType.hpp"
#include "glad/glad.h"
#include "logger/log.h"

#include "graphics_utils/LookAt.hpp"
#include "graphics_utils/CreateTexture.hpp"
#include "graphics_utils/DebugGlCall.hpp"

#include <string>
#include <cstring>

REGISTER_SINGLETON(Renderer)

void Renderer::render(EntityType type) const
{
    for (const auto& entity : _render_entities[static_cast<std::size_t>(type)])
    {
        // Interleaving vertex attributes instead of separate buffers for small performance boost from data locality:
        const auto *vertices = reinterpret_cast<const char *>(entity.vertices);
        const auto *uvs = vertices + 2 * sizeof(float);

        assert(vertices);

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
    for (std::size_t index = 0; index < static_cast<std::size_t>(EntityType::_SIZE); index++)
    {
        const auto& id_collection = _for_removal[index];
        auto& render_entities_collection = _render_entities[index];

        for (const auto &id : id_collection)
        {
            const auto predicate = [&id](const RenderEntity &e) { return e.id == id; };
            const auto it = std::remove_if(render_entities_collection.begin(), render_entities_collection.end(), predicate);
            if (it != render_entities_collection.end())
            {
                render_entities_collection.erase(it);
            }
        }
    }
}
