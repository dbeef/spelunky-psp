#include "prefabs/ui/CheatConsole.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ImguiComponent.hpp"
#include "logger/log.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"
#include "Input.hpp"

namespace {
    class CheatConsoleScript final : public ScriptBase {
    public:

        explicit CheatConsoleScript(entt::entity self) : _self(self) {}

        void update(entt::entity owner, uint32_t delta_time_ms) override {
            auto &registry = EntityRegistry::instance().get_registry();
            auto& input = Input::instance();

            if (input.cheat_console().changed() && input.cheat_console().value()) {
                _visible = !_visible;

                auto& imgui_component = registry.get<ImguiComponent>(_self);
                if (_visible) {
                    imgui_component.render_callback = &render_window;
                } else {
                    imgui_component.render_callback = &dont_render_window;
                }
            }
        }
    private:
        bool _visible = false;
        entt::entity _self;

        static void render_window() {
            bool show_another_window;
            ImGui::Begin("Cheat console", &show_another_window);
            ImGui::Text("This will work soon ;)");
            ImGui::End();
        }
        static void dont_render_window() {}
    };
}
namespace prefabs {

    entt::entity CheatConsole::create(const std::shared_ptr<Viewport> &viewport) {
        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();
        auto cheat_console_script = std::make_shared<CheatConsoleScript>(entity);
        ScriptingComponent scripting_component(cheat_console_script);
        registry.emplace<ScriptingComponent>(entity, scripting_component);

        ImguiComponent imgui_component;
        imgui_component.render_callback = [](){};
        registry.emplace<ImguiComponent>(entity, imgui_component);
        return entity;
    }
}