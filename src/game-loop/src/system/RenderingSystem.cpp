#include <components/generic/SortRenderingLayersComponent.hpp>
#include "system/RenderingSystem.hpp"
#include "EntityRegistry.hpp"
#include "Vertex.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "components/generic/TextComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/PositionComponent.hpp"
#include "components/generic/MeshComponent.hpp"

void RenderingSystem::update(std::uint32_t delta_time_ms)
{
    auto& registry = EntityRegistry::instance().get_registry();

    registry.view<QuadComponent>().each([&registry](entt::entity e, QuadComponent &quad) { quad.update(e); });
    registry.view<TextComponent>().each([&registry](entt::entity e, TextComponent &text) { text.update(e); });

    const auto sort_layers_components = registry.view<SortRenderingLayersComponent>();
    if (!sort_layers_components.empty())
    {
        sort_layers_components.each([&registry](entt::entity e){ registry.destroy(e); });
        sort();
    }

    auto meshes = registry.view<MeshComponent>();
    meshes.each([this](MeshComponent &mesh)
    {
        use_camera(mesh.camera_type);

        // Interleaving vertex attributes instead of separate buffers for small performance boost from data locality:
        const auto *vertices = reinterpret_cast<const char *>(mesh.vertices);
        const auto *uvs = vertices + 2 * sizeof(float);

        assert(vertices);

        // Tightly packed Vertex type:
        const size_t stride = sizeof(Vertex);

        DebugGlCall(glBindTexture(GL_TEXTURE_2D, mesh.texture_id));

        DebugGlCall(glVertexPointer(2, GL_FLOAT, stride, vertices));
        DebugGlCall(glTexCoordPointer(2, GL_FLOAT, stride, uvs));

        DebugGlCall(glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_SHORT, mesh.indices));
    });
}

void RenderingSystem::sort()
{
    auto& registry = EntityRegistry::instance().get_registry();

    registry.sort<MeshComponent>([&registry](entt::entity& e1, entt::entity& e2) {
        const auto& m1 = registry.get<MeshComponent>(e1);
        const auto& m2 = registry.get<MeshComponent>(e2);
        return static_cast<int>(m1.rendering_layer) < static_cast<int>(m2.rendering_layer) &&
               static_cast<int>(m1.camera_type) <= static_cast<int>(m2.camera_type);
    });
}

void RenderingSystem::use_camera(CameraType camera_type)
{
    if (camera_type == _cameras.current)
    {
        return;
    }

    switch(camera_type)
    {
        case CameraType::SCREEN_SPACE: use_screen_space_camera(); break;
        case CameraType::MODEL_VIEW_SPACE: use_model_view_camera(); break;
        default: assert(false); break;
    }
}

void RenderingSystem::use_model_view_camera()
{
    _cameras.current = CameraType::MODEL_VIEW_SPACE;
    _cameras.model_view.update_gl_modelview_matrix();
    _cameras.model_view.update_gl_projection_matrix();
}

void RenderingSystem::use_screen_space_camera()
{
    _cameras.current = CameraType::SCREEN_SPACE;
    _cameras.screen_space.update_gl_modelview_matrix();
    _cameras.screen_space.update_gl_projection_matrix();
}

RenderingSystem::RenderingSystem(const std::shared_ptr<Viewport> &viewport)
        : _cameras{CameraType::NONE, {viewport}, {viewport, ScreenSpaceCamera::CoordinateType::WORLD_UNITS}}
{
    _cameras.model_view.calculate_coefficients();
    _cameras.screen_space.calculate_coefficients();
    use_camera(CameraType::SCREEN_SPACE);
}

