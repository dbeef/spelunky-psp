#include "system/RenderingSystem.hpp"
#include "EntityRegistry.hpp"
#include "Vertex.hpp"
#include "glad/glad.h"
#include "graphics_utils/DebugGlCall.hpp"
#include "components/generic/TextComponent.hpp"
#include "components/generic/QuadComponent.hpp"
#include "components/generic/MeshComponent.hpp"
#include "components/generic/BlinkingComponent.hpp"
#include "components/generic/ImguiComponent.hpp"

void RenderingSystem::update(std::uint32_t delta_time_ms)
{
    update_opengl(delta_time_ms);
    update_imgui();
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

void RenderingSystem::update_blinking(std::uint32_t delta_time_ms) const
{
    auto& registry = EntityRegistry::instance().get_registry();
    registry.view<BlinkingComponent, QuadComponent>().each([&](entt::entity e,
                                                               BlinkingComponent& blinking,
                                                               QuadComponent& quad)
   {
       // In case changed:

       if (quad.get_quad_width() != 0.0f)
       {
           blinking.set_original_quad_width(quad.get_quad_width());
       }

       if (quad.get_quad_height() != 0.0f)
       {
           blinking.set_original_quad_height(quad.get_quad_height());
       }

       blinking.update(delta_time_ms);

       // Now actual blinking:

       if (blinking.is_transparent() && !blinking.is_for_removal())
       {
           quad.set_quad_width(0.0f);
           quad.set_quad_height(0.0f);
       }
       else
       {
           quad.set_quad_width(blinking.get_original_quad_width());
           quad.set_quad_height(blinking.get_original_quad_height());
       }

       if (blinking.is_for_removal())
       {
           registry.remove<BlinkingComponent>(e);
       }
   });
}

void RenderingSystem::update_opengl(std::uint32_t delta_time_ms) {
    auto& registry = EntityRegistry::instance().get_registry();

    registry.view<QuadComponent>().each([](entt::entity e, QuadComponent &quad) { quad.update(e); });
    registry.view<TextComponent>().each([](entt::entity e, TextComponent &text) { text.update(e); });

    update_blinking(delta_time_ms);
    sort(); // FIXME: Sorting components every frame!

    // FIXME: Pointers to meshes may become invalidated after entity pool resize. Same as BombSpawner.cpp:31.
    //        Temporary workaround at GameLoopStartedState.cpp:35 - reserving quads/meshes up-front.
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

#if defined(SPELUNKY_PSP_WITH_IMGUI)
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"

void RenderingSystem::update_imgui() {

    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    auto& registry = EntityRegistry::instance().get_registry();
    auto imguis = registry.view<ImguiComponent>();
    imguis.each([](ImguiComponent& imgui_component)
    {
        imgui_component.render_callback();
    });

    ImGui::Render();

    auto* draw_data = ImGui::GetDrawData();
    if (draw_data) {
        ImGui_ImplOpenGL2_RenderDrawData(draw_data);
    }
}
#else
void RenderingSystem::update_imgui() {}
#endif
