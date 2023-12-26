#include "prefabs/ui/CheatConsole.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ImguiComponent.hpp"
#include "logger/log.h"
#include "Input.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "other/CheatConsoleInterpreter.h"
#include <sstream>

// TODO: Better way to handle this than if-defing as there will be more imgui-components
#if defined(SPELUNKY_PSP_WITH_IMGUI)
#include "imgui_impl_opengl2.h"
#include "spritesheet-frames/CaveLevelSpritesheetFrames.hpp"

namespace {
    std::vector<std::string> split_by_space(const std::string& line)
    {
        std::stringstream ss_line(line);
        std::vector<std::string> seglist;
        std::string segment;

        while(std::getline(ss_line, segment, ' '))
        {
            seglist.push_back(segment);
        }

        return seglist;
    }

    // Based on ImGui's example console
    static int Stricmp(const char *s1, const char *s2) {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++;
            s2++;
        }
        return d;
    }

    static char *Strdup(const char *s) {
        IM_ASSERT(s);
        size_t len = strlen(s) + 1;
        void *buf = malloc(len);
        IM_ASSERT(buf);
        return (char *) memcpy(buf, (const void *) s, len);
    }

    static void Strtrim(char *s) {
        char *str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ') str_end--;
        *str_end = 0;
    }

    struct ImGuiConsole {
        char InputBuf[256];
        ImVector<char *> Items;
        ImVector<const char *> Commands;
        ImVector<char *> History;
        bool AutoScroll;
        bool ScrollToBottom;

        std::vector<CheatConsoleInterpreter::CommandHandler> _command_handlers;
        
        void add_command_handler(const CheatConsoleInterpreter::CommandHandler& command_handler) {
            _command_handlers.push_back(command_handler);    
        }
        
        ImGuiConsole() {
            ClearLog();
            memset(InputBuf, 0, sizeof(InputBuf));
            // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
            Commands.push_back("HELP");
            Commands.push_back("HISTORY");
            Commands.push_back("CLEAR");
            Commands.push_back("CLASSIFY");
            AutoScroll = true;
            ScrollToBottom = false;
        }

        ~ImGuiConsole() {
            ClearLog();
            for (int i = 0; i < History.Size; i++)
                free(History[i]);
        }

        void ClearLog() {
            for (int i = 0; i < Items.Size; i++)
                free(Items[i]);
            Items.clear();
        }

        void AddLog(const char *fmt, ...) IM_FMTARGS(2) {
            // FIXME-OPT
            char buf[1024];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
            buf[IM_ARRAYSIZE(buf) - 1] = 0;
            va_end(args);
            Items.push_back(Strdup(buf));
        }

        void Draw(const char *title, bool *p_open) {
            ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin(title, p_open)) {
                ImGui::End();
                return;
            }

            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Close Console")) *p_open = false;
                ImGui::EndPopup();
            }

            ImGui::Separator();
            ImGui::SetWindowFontScale(2.0f);

            // Reserve enough left-over height for 1 separator + 1 input text
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiWindowFlags_HorizontalScrollbar)) {
                if (ImGui::BeginPopupContextWindow()) {
                    if (ImGui::Selectable("Clear")) ClearLog();
                    ImGui::EndPopup();
                }

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
                for (const char *item: Items) {
                    // Normally you would store more information in your item than just a string.
                    // (e.g. make Items[] an array of structure, store color/type etc.)
                    ImVec4 color;
                    bool has_color = false;
                    if (strstr(item, "[error]")) {
                        color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                        has_color = true;
                    } else if (strncmp(item, "# ", 2) == 0) {
                        color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                        has_color = true;
                    }
                    if (has_color)
                        ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::TextUnformatted(item);
                    if (has_color)
                        ImGui::PopStyleColor();
                }

                // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
                // Using a scrollbar or mouse-wheel will take away from the bottom edge.
                if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                    ImGui::SetScrollHereY(1.0f);
                ScrollToBottom = false;
                ImGui::PopStyleVar();
            }
            ImGui::EndChild();
            ImGui::Separator();

            // Command-line
            bool reclaim_focus = false;
            ImGuiInputTextFlags input_text_flags =
                    ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll;
            ImGui::SetKeyboardFocusHere(0); // Added
            if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, nullptr, (void *) this)) {
                char *s = InputBuf;
                Strtrim(s);
                if (s[0])
                    ExecCommand(s);
                strcpy(s, "");
                reclaim_focus = true;
            }

            // Auto-focus on window apparition
            ImGui::SetItemDefaultFocus();
            if (reclaim_focus) {
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
            }

            ImGui::End();

            // TODO: Move this section out to a separate window, added this just for testing

            ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
            ImGui::Begin("Tiles", p_open);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 0.25f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));

            std::size_t tex_index = 0;
            std::size_t last_tex_index = static_cast<int>(CaveLevelSpritesheetFrames::_SIZE);

            while (tex_index < last_tex_index) {
                TextureID tiles_texture = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);
                auto door_texture = TextureBank::instance().get_region(TextureType::CAVE_LEVEL_TILES, tex_index);
                ImGui::SameLine();

                ImGui::ImageButton(
                        reinterpret_cast<void *>(tiles_texture),
                        ImVec2((float) door_texture.width * 4, (float) door_texture.height * 4),
                        ImVec2(door_texture.uv_normalized[0][0], door_texture.uv_normalized[0][1]),
                        ImVec2(door_texture.uv_normalized[2][0], door_texture.uv_normalized[2][1]),
                        1
                );
                tex_index++;

                if (tex_index % 6) {
                    ImGui::SameLine();
                } else
                {
                    ImGui::NewLine();
                }
            }

            ImGui::PopStyleColor(3);
            ImGui::End();
        }

        void ExecCommand(const char *command_line) {
            AddLog("# %s\n", command_line);

            // Insert into history. First find match and delete it so it can be pushed to the back.
            // This isn't trying to be smart or optimal.
            for (int i = History.Size - 1; i >= 0; i--) {
                if (Stricmp(History[i], command_line) == 0) {
                    free(History[i]);
                    History.erase(History.begin() + i);
                    break;
                }
            }
            History.push_back(Strdup(command_line));

            std::string command_str(command_line);
            std::string command_str_upper;
            std::transform(command_str.cbegin(), command_str.cend(), std::back_inserter(command_str_upper), [](char c){ return std::toupper(c);});
            const CheatConsoleInterpreter::Command command = split_by_space(command_str_upper);

            // Process command
            bool handled = false;
            bool any_logs = false;
            for (auto& handler : _command_handlers) {
                const auto result = handler(command);
                if (!result.second.empty())
                {
                    AddLog(result.second.c_str());
                    any_logs = true;
                }
                if (result.first)
                {
                    handled = true;
                    break;
                }
            }

            if (handled || any_logs) {
                // Do nothing
            } else if (Stricmp(command_line, "CLEAR") == 0) {
                ClearLog();
            } else if (Stricmp(command_line, "HELP") == 0) {
                AddLog("Commands:");
                for (int i = 0; i < Commands.Size; i++) {
                    AddLog("- %s", Commands[i]);
                }
            } else if (Stricmp(command_line, "HISTORY") == 0) {
                int first = History.Size - 10;
                for (int i = first > 0 ? first : 0; i < History.Size; i++) {
                    AddLog("%3d: %s\n", i, History[i]);
                }
            } else {
                AddLog("Unknown command: '%s'\n", command_line);
            }

            // On command input, we scroll to bottom even if AutoScroll==false
            ScrollToBottom = true;
        }
    };
    class CheatConsoleScript final : public ScriptBase {
    public:
        explicit CheatConsoleScript(entt::entity self) : _self(self) {
            _imgui_console.add_command_handler(_cheat_console_interpreter.get_enter_command_handler());
            _imgui_console.add_command_handler(_cheat_console_interpreter.get_spawn_command_handler());
            _dont_render_callback = [](){};
            _render_console_callback = [this](){
                bool dummy = false;
                _imgui_console.Draw("Cheat console", &dummy);
            };
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override {
            auto &registry = EntityRegistry::instance().get_registry();
            auto &input = Input::instance();
            if (input.cheat_console().changed() && input.cheat_console().value()) {
                _visible = !_visible;
                auto &imgui_component = registry.get<ImguiComponent>(_self);
                if (_visible) {
                    imgui_component.render_callback = _render_console_callback;
                } else {
                    imgui_component.render_callback = _dont_render_callback;
                }
            }
        }

    private:
        entt::entity _self;
        ImGuiConsole _imgui_console;
        bool _visible = false;
        std::function<void()> _render_console_callback;
        std::function<void()> _dont_render_callback;
        CheatConsoleInterpreter _cheat_console_interpreter;
    };
}
#else
class CheatConsoleScript final : public ScriptBase {
    public:
        explicit CheatConsoleScript(entt::entity self) { }
        void update(entt::entity owner, uint32_t delta_time_ms) override { }
};
#endif

namespace prefabs {

    entt::entity CheatConsole::create(const std::shared_ptr<Viewport> &viewport) {
        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();
        auto cheat_console_script = std::make_shared<CheatConsoleScript>(entity);
        CheatConsoleComponent cheat_console_component{};
        ScriptingComponent scripting_component(cheat_console_script);
        registry.emplace<ScriptingComponent>(entity, scripting_component);

        ImguiComponent imgui_component;
        imgui_component.render_callback = [](){};
        registry.emplace<ImguiComponent>(entity, imgui_component);
        registry.emplace<CheatConsoleComponent>(entity, cheat_console_component);
        return entity;
    }
}