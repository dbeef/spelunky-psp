#include "prefabs/ui/CheatConsole.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ImguiComponent.hpp"
#include "logger/log.h"
#include "Input.hpp"
#include "prefabs/npc/RedFrog.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include "game-loop/GameLoop.hpp"

// TODO: Better way to handle this than if-defing as there will be more imgui-components
#if defined(SPELUNKY_PSP_WITH_IMGUI)
#include "imgui_impl_opengl2.h"
#include "imgui_impl_sdl2.h"

namespace {
    // Based on ImGui's example console:
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

    using Command = std::string;
    using CommandHandlerResult = std::pair<bool, std::string>;
    using CommandHandler = std::function<CommandHandlerResult(const Command&)>;

    struct ImGuiConsole {
        char InputBuf[256];
        ImVector<char *> Items;
        ImVector<const char *> Commands;
        ImVector<char *> History;
        ImGuiTextFilter Filter;
        bool AutoScroll;
        bool ScrollToBottom;

        std::vector<CommandHandler> _command_handlers;
        
        void add_command_handler(const CommandHandler& command_handler) {
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
                if (ImGui::MenuItem("Close Console"))
                    *p_open = false;
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
                    if (!Filter.PassFilter(item))
                        continue;

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
            if (reclaim_focus)
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

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

            // Process command
            bool handled = false;
            for (auto& handler : _command_handlers) {
                const auto result = handler(command_line);
                if (result.first)
                {
                    handled = true;
                    AddLog(result.second.c_str());
                    break;
                }
            }

            if (handled) {
                // ...
            }
            else if (Stricmp(command_line, "CLEAR") == 0) {
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
            // TODO: Helper for splitting into tokens
            _imgui_console.add_command_handler([](const Command& command){
                Command upper_cased;
                std::transform(command.cbegin(), command.cend(), std::back_inserter(upper_cased), [](char c){ return std::toupper(c);});
                if (upper_cased == "EXIT") {
                    return std::make_pair(true, "exiting");
                } else {
                    return std::make_pair(false, "");
                }
            });
            _imgui_console.add_command_handler([this](const Command& command){
                Command upper_cased;
                std::transform(command.cbegin(), command.cend(), std::back_inserter(upper_cased), [](char c){ return std::toupper(c);});
                if (upper_cased == "ENTER SCORES") {
                    auto& registry = EntityRegistry::instance().get_registry();
                    auto& cheat_console_component = registry.get<prefabs::CheatConsoleComponent>(_self);
                    cheat_console_component.request_state_change(GameLoop::State::SCORES);
                    return std::make_pair(true, "entering scores");
                } else if (upper_cased == "ENTER MAIN_MENU") {
                    auto& registry = EntityRegistry::instance().get_registry();
                    auto& cheat_console_component = registry.get<prefabs::CheatConsoleComponent>(_self);
                    cheat_console_component.request_state_change(GameLoop::State::MAIN_MENU);
                    return std::make_pair(true, "entering main_menu");
                } else if (upper_cased == "ENTER SANDBOX") {
                    auto& registry = EntityRegistry::instance().get_registry();
                    auto& cheat_console_component = registry.get<prefabs::CheatConsoleComponent>(_self);
                    cheat_console_component.request_state_change(GameLoop::State::SANDBOX);
                    return std::make_pair(true, "entering sanbox");
                } else if (upper_cased == "ENTER PLAYING") {
                    auto& registry = EntityRegistry::instance().get_registry();
                    auto& cheat_console_component = registry.get<prefabs::CheatConsoleComponent>(_self);
                    cheat_console_component.request_state_change(GameLoop::State::PLAYING);
                    return std::make_pair(true, "entering playing");
                } else {
                    return std::make_pair(false, "");
                }
            });
            _imgui_console.add_command_handler([](const Command& command){
                Command upper_cased;
                std::transform(command.cbegin(), command.cend(), std::back_inserter(upper_cased), [](char c){ return std::toupper(c);});
                if (upper_cased == "SPAWN REDFROG") {

                    auto& registry = EntityRegistry::instance().get_registry();
                    auto dudes = registry.view<MainDudeComponent>();
                    assert(dudes.size() == 1);
                    auto dude = dudes.front();
                    auto &dude_position = registry.get<PositionComponent>(dude);

                    const float pos_x = dude_position.x_center - 2;
                    const float pos_y = dude_position.y_center - 2;
                    prefabs::RedFrog::create(pos_x, pos_y);

                    return std::make_pair(true, "spawning");
                } else {
                    return std::make_pair(false, "");
                }
            });
        }

        void update(entt::entity owner, uint32_t delta_time_ms) override {
            auto &registry = EntityRegistry::instance().get_registry();
            auto &input = Input::instance();
            if (input.cheat_console().changed() && input.cheat_console().value()) {
                _visible = !_visible;
                auto &imgui_component = registry.get<ImguiComponent>(_self);
                if (_visible) {
                    imgui_component.render_callback = render_console_callback;
                } else {
                    imgui_component.render_callback = dont_render_callback;
                }
            }
        }

    private:
        // Returns commands, i.e <SPAWN, REDFROG>
        //                   i.e <ENTER, SCORES>
        // To be interpreted later

        entt::entity _self;
        ImGuiConsole _imgui_console;
        bool _visible = false;
        const std::function<void()> render_console_callback = [this](){
            bool dummy = false;
            _imgui_console.Draw("Cheat console", &dummy);
        };
        const std::function<void()> dont_render_callback = [](){};
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