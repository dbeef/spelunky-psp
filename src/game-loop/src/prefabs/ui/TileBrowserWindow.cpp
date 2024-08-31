#include "prefabs/ui/TileBrowserWindow.hpp"
#include "EntityRegistry.hpp"
#include "components/generic/ScriptingComponent.hpp"
#include "components/generic/ImguiComponent.hpp"
#include "logger/log.h"
#include "Input.hpp"
#include "components/specialized/MainDudeComponent.hpp"
#include <sstream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "cJSON.h"
#include "Level.hpp"

// TODO: Groups could make use of a table (1 row name, 1 row the buttons - focus/delete), that's instead of bullet points
// TODO: Instead of elaborate line with all the parameters for adding, just a single "+" that opens a modal pop-up
// TODO: Use tooltip instead of displaying room size (i.e 10x10)
// TODO: Entity tracking with: https://github.com/skypjack/entt/issues/98
// TODO: Different width of the CheatConsole depending on location (main menu / editor)
// TODO: On save prompt (saved xxx bytes)

// TODO: Better way to handle this than if-defing as there will be more imgui-components
#if defined(SPELUNKY_PSP_WITH_IMGUI)

#include "imgui_impl_opengl2.h"
#include "spritesheet-frames/CaveLevelSpritesheetFrames.hpp"
#include "TileBatch.hpp"

// TODO: Command queue pattern

int MSG_DEFAULT_TIMEOUT = 2000;
int MSG_TIMEOUT_INFINITE = std::numeric_limits<int>::max();
ImVec4 MSG_COLOR_NEUTRAL = {1.0f, 1.0f, 1.0f, 1.0f};
ImVec4 MSG_COLOR_OK = {0.5f, 1.0f, 0.0f, 1.0f};
ImVec4 MSG_COLOR_ERROR = {1.0f, 0.3f, 0.3f, 1.0f};

struct TimedPromptMessage {
    ImVec4 color;
    std::string contents;
    int timer_ms = MSG_DEFAULT_TIMEOUT;
};

class Prompt {
public:

    std::string get() {
        std::stringstream out;
        std::for_each(messages.begin(), messages.end(), [&out](const auto& msg) { out << msg.contents; });
        return out.str();
    }

    const std::vector<TimedPromptMessage>& get_messages() const {
        return messages;
    }

    void update(std::uint16_t delta_time_ms) {
        for (auto& msg : messages) {
            msg.timer_ms -= delta_time_ms;
        }
        auto iter = std::remove_if(messages.begin(), messages.end(), [](const auto& msg) { return msg.timer_ms <= 0; });
        if (iter != messages.end()) {
            messages.erase(iter);
        }
    }

    void push_message(const TimedPromptMessage& msg) {
        messages.push_back(msg);
    }
private:
    std::vector<TimedPromptMessage> messages;
};

static Prompt prompt;

namespace {

    entt::entity tile_batch_entity = entt::null;
    struct {
        int width = Consts::ROOM_WIDTH_TILES;
        int height = Consts::ROOM_HEIGHT_TILES;
        std::string name = "";
    } ui_new_room;

    struct {
        std::string name = "";
    } ui_new_group;

    struct {
        bool delete_popup_visible{};
    } ui_existing_room;

    auto assets = std::filesystem::current_path() / "../assets/rooms";

    struct Room {
        std::string name;
        int width_tiles{};
        int height_tiles{};
        std::vector<MapTileType> tiles;

        Room() = default;

        explicit Room(const std::string &name, int width, int height) : width_tiles(width), height_tiles(height),
                                                                        name(name) {
            tiles.resize(width * height, MapTileType::NOTHING);
        }

        MapTileType &get_xy(int x, int y) {
            return tiles[(y * width_tiles) + x];
        }

        void set_xy(int x, int y, MapTileType value) {
            get_xy(x, y) = value;
        }

        void from_json(cJSON *in_json) {
            auto *name_cjson = cJSON_GetObjectItem(in_json, "name");
            auto *width_tiles_cjson = cJSON_GetObjectItem(in_json, "width_tiles");
            auto *height_tiles_cjson = cJSON_GetObjectItem(in_json, "height_tiles");
            auto *tiles_cjson = cJSON_GetObjectItem(in_json, "tiles");

            assert(cJSON_IsString(name_cjson));
            assert(cJSON_IsNumber(width_tiles_cjson));
            assert(cJSON_IsNumber(height_tiles_cjson));

            name = name_cjson->valuestring;
            width_tiles = width_tiles_cjson->valueint;
            height_tiles = height_tiles_cjson->valueint;

            if (tiles_cjson) {
                auto array_size = cJSON_GetArraySize(tiles_cjson);
                tiles.resize(array_size);

                for (int i = 0; i < array_size; i++) {
                    auto *item = cJSON_GetArrayItem(tiles_cjson, i);
                    tiles[i] = static_cast<MapTileType>(item->valueint);
                }
            }
        }

        cJSON *to_json() {
            auto *room = cJSON_CreateObject();
            cJSON_AddStringToObject(room, "name", name.c_str());
            cJSON_AddNumberToObject(room, "width_tiles", width_tiles);
            cJSON_AddNumberToObject(room, "height_tiles", height_tiles);

//            tiles = {MapTileType::STONE_BLOCK, MapTileType::ALTAR_RIGHT};

            auto *rooms_array = cJSON_CreateIntArray(reinterpret_cast<const int *>(tiles.data()), tiles.size());
            cJSON_AddItemToObject(room, "tiles", rooms_array);

            return room;
        }
    };

    struct RoomGroup {
        std::string name;
        std::vector<Room> rooms;

        void from_json(cJSON *in_json) {
            auto *name_cjson = cJSON_GetObjectItem(in_json, "name");
            assert(cJSON_IsString(name_cjson));
            name = name_cjson->valuestring;

            auto *rooms_cjson = cJSON_GetObjectItem(in_json, "rooms");
            if (rooms_cjson) {
                auto array_size = cJSON_GetArraySize(rooms_cjson);
                rooms.resize(array_size);
                for (int i = 0; i < array_size; i++) {
                    auto *item = cJSON_GetArrayItem(rooms_cjson, i);
                    rooms[i].from_json(item);
                }
            }
        }

        cJSON *to_json() {
            auto *group = cJSON_CreateObject();
            cJSON_AddStringToObject(group, "name", name.c_str());

            auto *rooms_array = cJSON_CreateArray();

            for (auto &r: rooms) {
                auto *cjson_room = r.to_json();
                cJSON_AddItemToArray(rooms_array, cjson_room);
            }

            cJSON_AddItemToObject(group, "rooms", rooms_array);

            return group;
        }
    };

    void load_room_tiles(Room &r) {
        // FIXME: Sometimes not valid because TileBrowserWindow entity is disposed >after< rendering entity is already disposed
        if (tile_batch_entity != entt::null && EntityRegistry::instance().get_registry().valid(tile_batch_entity)) {
            EntityRegistry::instance().get_registry().destroy(tile_batch_entity);
            tile_batch_entity = entt::null;
        }

        Level::instance().recreate_tile_batch(r.width_tiles, r.height_tiles);
        auto &tile_batch = Level::instance().get_tile_batch();

        auto &tiles = tile_batch.map_tiles;
        for (int i_x = 0; i_x < r.width_tiles; i_x++) {
            // Camel Humps + Ctrl+W on a single camel + Double-Ctrl+W for full phrase - I want it in vim!
            for (int i_y = 0; i_y < r.height_tiles; i_y++) {
                tiles[i_x][i_y]->match_tile(r.get_xy(i_x, i_y));
            }
        }
        tile_batch.batch_vertices();
        tile_batch_entity = tile_batch.add_render_entity(EntityRegistry::instance().get_registry());
    }

    void save_room_tiles(Room &r) {
        auto &tile_batch = Level::instance().get_tile_batch();
        auto &tiles = tile_batch.map_tiles;
        for (int i_x = 0; i_x < r.width_tiles; i_x++) {
            // Camel Humps + Ctrl+W on a single camel + Double-Ctrl+W for full phrase - I want it in vim!
            for (int i_y = 0; i_y < r.height_tiles; i_y++) {
                r.set_xy(i_x, i_y, tiles[i_x][i_y]->map_tile_type);
            }
        }
        tile_batch.batch_vertices();
    }

    std::vector<RoomGroup> all_groups;
    int room_focus_idx = 0;
    int current_group_idx = 0;
    std::size_t room_to_delete_idx = 0;
}

namespace {
    class TileBrowserScript final : public ScriptBase {
    public:
        explicit TileBrowserScript(entt::entity self, const std::shared_ptr<Viewport>& viewport) : _self(self), _viewport(viewport) {

            ui_new_group.name.resize(4096);
            ui_new_room.name.resize(4096);

            // TODO: Move reading/parsing out to a separate file

            std::ifstream in(assets / "groups.json", std::ifstream::in | std::ifstream::ate);
            if (in.good()) {
                std::size_t size = in.tellg();
                in.seekg(0);

                std::vector<char> in_chars(size);
                in.read(in_chars.data(), size);

                auto *in_json = cJSON_Parse(in_chars.data());
                log_info(cJSON_PrintUnformatted(in_json));

                auto *groups = cJSON_GetObjectItem(in_json, "groups");
                assert(cJSON_IsArray(groups));
                auto array_size = cJSON_GetArraySize(groups);

                all_groups = {};
                for (int i = 0; i < array_size; i++) {
                    auto *item = cJSON_GetArrayItem(groups, i);
                    RoomGroup in_rg;
                    in_rg.from_json(item);
//                    log_info("Room is: %i x %i", in_r.width_tiles, in_r.height_tiles);;
                    all_groups.push_back(in_rg);
                }

                cJSON_Delete(in_json);
            }

            if (!all_groups.empty() && !all_groups.begin()->rooms.empty()) {
                load_room_tiles(all_groups.at(0).rooms.at(0));
            }

            _dont_render_callback = [](int delta_time_ms) {};
            _render_console_callback = [this](int delta_time_ms) {
                bool open = false;


                ImGui::SetNextWindowPos(ImVec2(0.0f, _viewport->get_height_pixels() * 0.965f));
                ImGui::SetNextWindowSize(ImVec2(_viewport->get_width_pixels() * 1.0f, _viewport->get_height_pixels() * 0.04f), ImGuiCond_Always);

                ImGui::Begin("FileInfo", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                ImGui::SetWindowFontScale(2.0f); // FIXME: Blurry

                ImGui::Text("Editing:");
                ImGui::SameLine();
                // Exists - GREEN
                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "%s", assets.c_str());
                ImGui::SameLine();
                // Non-zero - GREEN
//                ImGui::Text("File size:");
//                ImGui::SameLine();
//                ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "%i bytes", std::filesystem::file_size(assets / "groups.json"));
//                ImGui::SameLine();

                ImGui::SameLine();
                if (ImGui::SmallButton("Save")) {

                    prompt.push_message({MSG_COLOR_NEUTRAL, "Saved"});

                    if (!all_groups.at(current_group_idx).rooms.empty()) {
                        save_room_tiles(all_groups.at(current_group_idx).rooms.at(room_focus_idx));
                    }

                    cJSON *out_json = cJSON_CreateObject();
                    cJSON_AddStringToObject(out_json, "SpelunkyPSP_Rooms_LastEdited", "dummy_date");

                    auto *rooms_array = cJSON_AddArrayToObject(out_json, "groups");
                    for (auto &g: all_groups) {
                        cJSON_AddItemToArray(rooms_array, g.to_json());
                    }

                    log_info(cJSON_PrintUnformatted(out_json));
                    log_info(assets.c_str());
                    log_info("%i", std::filesystem::exists(assets));

                    auto out_str = std::ofstream(assets / "groups.json", std::ofstream::out);
                    out_str << cJSON_PrintUnformatted(out_json);

                    cJSON_Delete(out_json);
                }


                prompt.update(delta_time_ms);

                ImGui::SameLine();
                // No - GREEN
                // ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "(Unsaved changes)");
                // ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), "(No changes)");
                // ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.0f, 1.0f), prompt.get().c_str());

                for (auto& msg : prompt.get_messages()) {
                    ImGui::TextColored(MSG_COLOR_NEUTRAL, " * ");
                    ImGui::SameLine();
                    ImGui::TextColored(msg.color, msg.contents.c_str());
                    ImGui::SameLine();
                }

                ImGui::End();

                ImGui::SetNextWindowSize(ImVec2(_viewport->get_width_pixels() * 0.4f, _viewport->get_height_pixels() * 0.96f), ImGuiCond_Always);
                ImGui::SetNextWindowPos(ImVec2(_viewport->get_width_pixels() * 0.6f, 0));
//                ImGui::Begin("Tiles", nullptr);
                ImGui::Begin("Tiles", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
                ImGui::SetWindowFontScale(2.0f); // FIXME: Blurry


//                if (ImGui::TreeNode("Child windows")) {
//                    for (auto &r: all_rooms) {
//                        ImGui::Bullet();
//                        ImGui::Text(r.name.c_str());
//                    }
//                }
//                ImGui::TreePop();

                std::size_t group_id = 0;
                bool pop_color = false;

                for (auto &g: all_groups) {

                    if (current_group_idx == group_id) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1.0f, 0.0f, 1.0f));
                        pop_color = true;
                    }

                    if (ImGui::TreeNode(g.name.c_str())) {

                        if (pop_color) {
                            ImGui::PopStyleColor();
                            pop_color = false;
                        }

                        auto &all_rooms = g.rooms;
//                    ImGui::BulletText("Bullet point 1");
//                    ImGui::BulletText("Bullet point 2\nOn multiple lines");
                        int ii = 0;
                        bool pop = false;
                        for (auto &r: all_rooms) {
                            ImGui::PushID(ii);
                            if (ii == room_focus_idx && group_id == current_group_idx) {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1.0f, 0.0f, 1.0f));
                                pop = true;
                            }
                            ImGui::Bullet();

                            ImGui::Text("%s (%i x %i)", r.name.c_str(), r.width_tiles, r.height_tiles);
                            ImGui::SameLine();
                            if (ImGui::SmallButton("Focus")) {
                                if (!all_groups.at(current_group_idx).rooms.empty()) {
                                    save_room_tiles(all_groups.at(current_group_idx).rooms.at(room_focus_idx));
                                }
                                room_focus_idx = ii;
                                current_group_idx = group_id;
                                load_room_tiles(r);
                            }
                            ImGui::SameLine();


                            static int selected_fish = -1;
                            const char* names[] = { "Yes", "Take me back :(" };
                            static bool toggles[] = { true, false };

                            // Simple selection popup (if you want to show the current selection inside the Button itself,
                            // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
                            ImGui::SameLine();
                            if (ImGui::SmallButton("Delete")) {
                                ImGui::OpenPopup("my_select_popup");
                            }

//                            ImGui::SameLine();
                            if (ImGui::BeginPopup("my_select_popup"))
                            {
                                ImGui::SeparatorText("Really?");
                                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                                    if (ImGui::Selectable(names[i]))
                                        selected_fish = i;
                                ImGui::EndPopup();
                            }


//                            if (ImGui::SmallButton("Delete")) {
//                                room_to_delete_idx = ii;
//                                ui_existing_room.delete_popup_visible = true;
//                            }

                            if (pop) {
                                ImGui::PopStyleColor();
                                pop = false;
                            }

                            ii++;
                            ImGui::PopID();
                        }

//                    ImGui::Bullet(); ImGui::Text("Bullet point 3 (two calls)");

                        ImGui::Bullet();

                        ImGui::SameLine();
                        ImGui::PushItemWidth(150);
                        ImGui::InputInt("Width", &ui_new_room.width);
                        ImGui::PopItemWidth();

                        ImGui::SameLine();
                        ImGui::PushItemWidth(150);
                        ImGui::InputInt("Height", &ui_new_room.height);
                        ImGui::PopItemWidth();

                        ImGui::PushItemWidth(145);
                        ImGui::SameLine();
                        char x = 0;
                        if (ImGui::InputTextWithHint("name", "<new room name>",
                                                     current_group_idx == group_id ? ui_new_room.name.data() : &x,
                                                     current_group_idx == group_id ? ui_new_room.name.capacity() : 0,
                                                     ImGuiInputTextFlags_EnterReturnsTrue)) {}
                        ImGui::PopItemWidth();

                        ImGui::SameLine();

                        if (ImGui::SmallButton("+") && ui_new_room.name.at(0) != '\0') {
                            if (!all_rooms.empty()) {
                                save_room_tiles(all_rooms.at(room_focus_idx));
                            }

                            auto &r = all_rooms.emplace_back(
                                    ui_new_room.name,
                                    ui_new_room.width,
                                    ui_new_room.height
                            );
                            room_focus_idx = all_rooms.size() - 1;
                            load_room_tiles(r);
                        }
                        ImGui::TreePop();
                    }
                    if (pop_color) {
                        ImGui::PopStyleColor();
                        pop_color = false;
                    }

                    group_id++;
                }

                ImGui::Bullet();
                ImGui::PushItemWidth(300);
                ImGui::SameLine();
                if (ImGui::InputTextWithHint("name", "<new group name>", ui_new_group.name.data(), ui_new_group.name.capacity(), ImGuiInputTextFlags_EnterReturnsTrue)) {
                }
                ImGui::PopItemWidth();
                ImGui::SameLine();

                if (ImGui::SmallButton("+")) {
                    if (ui_new_group.name.at(0) != '\0') {
                        auto &g = all_groups.emplace_back();
                        g.name = ui_new_group.name;
                        current_group_idx = all_groups.size() - 1;
                        log_info("%i", ui_new_group.name.size());
                        log_info("%i", ui_new_group.name.capacity());
                        log_info("%s", ui_new_group.name.c_str());
                        prompt.push_message({MSG_COLOR_NEUTRAL, "New group created: " + ui_new_group.name});
                    } else {
                        prompt.push_message({MSG_COLOR_ERROR, "Name can't be empty"});
                    }
                }

                ImGui::Separator();

                ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar("TilesheetTab", tab_bar_flags))
                {
                    if (ImGui::BeginTabItem("Cave"))
                    {
                        ImGui::Spacing();

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 0.25f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));

                        std::size_t tex_index = 0;
                        std::size_t last_tex_index = static_cast<int>(CaveLevelSpritesheetFrames::_SIZE);

                        while (tex_index < last_tex_index) {
                            ImGui::PushID(std::to_string((int) tex_index).c_str());
                            TextureID tiles_texture = TextureBank::instance().get_texture(TextureType::CAVE_LEVEL_TILES);
                            auto door_texture = TextureBank::instance().get_region(TextureType::CAVE_LEVEL_TILES, tex_index);
                            ImGui::SameLine();

                            ImGui::ImageButton(
                                    reinterpret_cast<ImTextureID>(tiles_texture),
                                    ImVec2((float) door_texture.width * 4, (float) door_texture.height * 4),
                                    ImVec2(door_texture.uv_normalized[0][0], door_texture.uv_normalized[0][1]),
                                    ImVec2(door_texture.uv_normalized[2][0], door_texture.uv_normalized[2][1]),
                                    1
                            );

                            if (ImGui::IsItemClicked()) {
                                auto &registry = EntityRegistry::instance().get_registry();
                                auto &tile_browser_window_component = registry.get<prefabs::TileBrowserWindowComponent>(_self);
                                tile_browser_window_component.set_selected_tile_type(MapTileType(tex_index));
                            }

                            tex_index++;

                            if (tex_index % 9) {
                                ImGui::SameLine();
                            } else {
                                ImGui::NewLine();
                            }
                            ImGui::PopID();
                        }

                        ImGui::PopStyleColor(3);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Jungle")) {ImGui::EndTabItem();}
                    if (ImGui::BeginTabItem("Ice")) {ImGui::EndTabItem();}
                    if (ImGui::BeginTabItem("Lava City")) {ImGui::EndTabItem();}
                    ImGui::EndTabBar();
                }

//                ImGui::End();
//                ImGui::SetNextWindowSize(ImVec2(_viewport->get_width_pixels() * 0.25f, _viewport->get_height_pixels() * 0.25f), ImGuiCond_Always);
//                ImGui::Begin("Room groups", &open);
                ImGui::End();

//                if (ui_existing_room.delete_popup_visible)
//                {
//                    ImGui::SetNextWindowSize(ImVec2(200, 80), ImGuiCond_Always);
//                    ImGui::Begin("Really delete?");
//
//                    if (ImGui::Button("Delete"))
//                    {
//                        all_rooms.erase(all_rooms.begin() + room_to_delete_idx);
//                        ui_existing_room.delete_popup_visible = false;
//                    }
//                    ImGui::SameLine();
//                    if (ImGui::Button("Cancel"))
//                    {
//                        ui_existing_room.delete_popup_visible = false;
//                    }
//                    ImGui::End();
//                }

            };

            if (!all_groups.empty())
            {
                auto& r = all_groups[current_group_idx].rooms.at(room_focus_idx);
                load_room_tiles(r);
            }
        }

        // TODO: Helpers - generate background

        void update(entt::entity owner, uint32_t delta_time_ms) override {
            auto &registry = EntityRegistry::instance().get_registry();
            auto &input = Input::instance();
            if (true) { // FIXME
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
        std::shared_ptr<Viewport> _viewport;
        bool _visible = true;
        std::function<void(int)> _render_console_callback;
        std::function<void(int)> _dont_render_callback;
    };
}
#else
class TileBrowserScript final : public ScriptBase {
    public:
        explicit TileBrowserScript(entt::entity self) { }
        void update(entt::entity owner, uint32_t delta_time_ms) override { }
};
#endif

namespace prefabs {
    entt::entity TileBrowserWindow::create(const std::shared_ptr<Viewport> &viewport) {
        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();
        auto cheat_console_script = std::make_shared<TileBrowserScript>(entity, viewport);
        TileBrowserWindowComponent cheat_console_component{};
        ScriptingComponent scripting_component(cheat_console_script);
        registry.emplace<ScriptingComponent>(entity, scripting_component);

        ImguiComponent imgui_component;
        imgui_component.render_callback = [](int delta_time_ms) {};
        registry.emplace<ImguiComponent>(entity, imgui_component);
        registry.emplace<TileBrowserWindowComponent>(entity, cheat_console_component);
        return entity;
    }
}