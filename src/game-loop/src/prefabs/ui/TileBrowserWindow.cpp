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

// TODO: Better way to handle this than if-defing as there will be more imgui-components
#if defined(SPELUNKY_PSP_WITH_IMGUI)
#include "imgui_impl_opengl2.h"
#include "spritesheet-frames/CaveLevelSpritesheetFrames.hpp"
#include "TileBatch.hpp"

namespace
{
    struct
    {
        int width = Consts::ROOM_WIDTH_TILES;
        int height = Consts::ROOM_HEIGHT_TILES;
        std::string name = "New room";
    } ui_new_room;

    struct
    {
        bool delete_popup_visible{};
    } ui_existing_room;

    auto assets = std::filesystem::current_path() / "../assets/rooms";

    struct Room
    {
        std::string name;
        int width_tiles{};
        int height_tiles{};
        std::vector<MapTileType> tiles;

        Room() = default;
        explicit Room(const std::string& name, int width, int height) : width_tiles(width), height_tiles(height), name(name)
        {
            tiles.resize(width * height, MapTileType::NOTHING);
        }

        MapTileType& get_xy(int x, int y)
        {
            return tiles[(y * width_tiles) + x];
        }

        void set_xy(int x, int y, MapTileType value)
        {
            get_xy(x, y) = value;
        }

        void from_json(cJSON* in_json)
        {
            auto* name_cjson = cJSON_GetObjectItem(in_json, "name");
            auto* width_tiles_cjson = cJSON_GetObjectItem(in_json, "width_tiles");
            auto* height_tiles_cjson = cJSON_GetObjectItem(in_json, "height_tiles");
            auto* tiles_cjson = cJSON_GetObjectItem(in_json, "tiles");

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

        cJSON* to_json()
        {
            auto* room = cJSON_CreateObject();
            cJSON_AddStringToObject(room, "name", name.c_str());
            cJSON_AddNumberToObject(room, "width_tiles", width_tiles);
            cJSON_AddNumberToObject(room, "height_tiles", height_tiles);

//            tiles = {MapTileType::STONE_BLOCK, MapTileType::ALTAR_RIGHT};

            auto* rooms_array = cJSON_CreateIntArray(reinterpret_cast<const int*>(tiles.data()), tiles.size());
            cJSON_AddItemToObject(room, "tiles", rooms_array);

            return room;
        }
    };

    struct RoomGroup
    {
        std::string name;
        std::vector<Room> rooms;

        void from_json(cJSON* in_json)
        {
            auto* name_cjson = cJSON_GetObjectItem(in_json, "name");
            assert(cJSON_IsString(name_cjson));
            name = name_cjson->valuestring;
        }

        cJSON* to_json()
        {
            auto* group = cJSON_CreateObject();
            cJSON_AddStringToObject(group, "name", name.c_str());

            auto* rooms_array = cJSON_CreateArray();

            for (auto& r : rooms){
                auto* cjson_room = r.to_json();
                cJSON_AddItemToArray(rooms_array, cjson_room);
            }

            cJSON_AddItemToObject(group, "rooms", rooms_array);

            return group;
        }
    };

    void load_room_tiles(Room& r)
    {
        auto& tile_batch = Level::instance().get_tile_batch();
        auto& tiles = tile_batch.map_tiles;
        for (int i_x = 0; i_x < r.width_tiles; i_x++) {
            // Camel Humps + Ctrl+W on a single camel + Double-Ctrl+W for full phrase - I want it in vim!
            for (int i_y = 0; i_y < r.height_tiles; i_y++){
                tiles[i_x][i_y]->match_tile(r.get_xy(i_x, i_y));
            }
        }
        tile_batch.batch_vertices();
    }

    void save_room_tiles(Room& r)
    {
        auto& tile_batch = Level::instance().get_tile_batch();
        auto& tiles = tile_batch.map_tiles;
        for (int i_x = 0; i_x < r.width_tiles; i_x++) {
            // Camel Humps + Ctrl+W on a single camel + Double-Ctrl+W for full phrase - I want it in vim!
            for (int i_y = 0; i_y < r.height_tiles; i_y++){
                r.set_xy(i_x, i_y, tiles[i_x][i_y]->map_tile_type);
            }
        }
        tile_batch.batch_vertices();
    }

    std::vector<Room> all_rooms;
    int focus_idx = 0;
    std::size_t room_to_delete_idx = 0;
}

namespace {
    class TileBrowserScript final : public ScriptBase {
    public:
        explicit TileBrowserScript(entt::entity self) : _self(self) {

            ui_new_room.name.resize(4096);

            std::ifstream in(assets/ "rooms.json", std::ifstream::in | std::ifstream::ate);
            if (in.good()) {
                std::size_t size = in.tellg();
                in.seekg(0);

                std::vector<char> in_chars(size);
                in.read(in_chars.data(), size);

                auto *in_json = cJSON_Parse(in_chars.data());
                log_info(cJSON_PrintUnformatted(in_json));

                auto *rooms = cJSON_GetObjectItem(in_json, "rooms");
                assert(cJSON_IsArray(rooms));
                auto array_size = cJSON_GetArraySize(rooms);

                all_rooms = {};
                for (int i = 0; i < array_size; i++) {
                    auto* item = cJSON_GetArrayItem(rooms, i);
                    Room in_r;
                    in_r.from_json(item);
                    log_info("Room is: %i x %i", in_r.width_tiles, in_r.height_tiles);;
                    all_rooms.push_back(in_r);
                }

                cJSON_Delete(in_json);
            }

            _dont_render_callback = [](){};
            _render_console_callback = [this](){
                bool open = false;
                ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_Always);
                ImGui::Begin("Tiles", &open);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 0.25f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));

                std::size_t tex_index = 0;
                std::size_t last_tex_index = static_cast<int>(CaveLevelSpritesheetFrames::_SIZE);

                while (tex_index < last_tex_index) {
                    ImGui::PushID(std::to_string((int)tex_index).c_str());
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
                        auto& registry = EntityRegistry::instance().get_registry();
                        auto& tile_browser_window_component = registry.get<prefabs::TileBrowserWindowComponent>(_self);
                        tile_browser_window_component.set_selected_tile_type(MapTileType(tex_index));
                    }

                    tex_index++;

                    if (tex_index % 6) {
                        ImGui::SameLine();
                    } else
                    {
                        ImGui::NewLine();
                    }
                    ImGui::PopID();
                }

                ImGui::PopStyleColor(3);

                ImGui::End();

                ImGui::SetNextWindowSize(ImVec2(520, 85), ImGuiCond_Always);
                ImGui::Begin("Save/Load", &open);

                if (ImGui::Button("Save"))
                {
                    if (!all_rooms.empty()) {
                        save_room_tiles(all_rooms.at(focus_idx));
                    }

                    cJSON* out_json = cJSON_CreateObject();
                    cJSON_AddStringToObject(out_json, "SpelunkyPSP_Rooms_LastEdited", "dummy_date");

                    auto* rooms_array = cJSON_AddArrayToObject(out_json, "rooms");
                    for (auto& r : all_rooms) {
                        cJSON_AddItemToArray(rooms_array, r.to_json());
                    }

                    log_info(cJSON_PrintUnformatted(out_json));
                    log_info(assets.c_str());
                    log_info("%i", std::filesystem::exists(assets));

                    auto out_str = std::ofstream(assets / "rooms.json", std::ofstream::out);
                    out_str << cJSON_PrintUnformatted(out_json);

                    cJSON_Delete(out_json);
                }
                ImGui::SameLine();

                ImGui::Text(assets.c_str());
                ImGui::End();

                ImGui::SetNextWindowSize(ImVec2(520, 400), ImGuiCond_Always);
                ImGui::Begin("Room groups", &open);

//                if (ImGui::TreeNode("Child windows")) {
//                    for (auto &r: all_rooms) {
//                        ImGui::Bullet();
//                        ImGui::Text(r.name.c_str());
//                    }
//                }
//                ImGui::TreePop();

                {
//                    ImGui::BulletText("Bullet point 1");
//                    ImGui::BulletText("Bullet point 2\nOn multiple lines");
                    int ii = 0;
                    bool pop = false;
                    for (auto& r : all_rooms) {
                        ImGui::PushID(ii);
                        if (ii == focus_idx) {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1.0f, 0.0f, 1.0f));
                            pop = true;
                        }
                        if (ImGui::TreeNode(r.name.c_str(), "%s (%i x %i)", r.name.c_str(), r.width_tiles, r.height_tiles)) {
                            ImGui::Bullet();
                            ImGui::Text("width %i", r.width_tiles);
                            ImGui::Bullet();
                            ImGui::Text("height %i", r.height_tiles);
                            ImGui::Bullet();
                            ImGui::Text("name %s", r.name.c_str());
                            ImGui::Bullet();
                            if (ImGui::SmallButton("Focus"))
                            {
                                save_room_tiles(all_rooms.at(focus_idx));
                                focus_idx = ii;
                                load_room_tiles(r);
                            }
                            ImGui::SameLine();
                            if (ImGui::SmallButton("Delete"))
                            {
                                room_to_delete_idx = ii;
                                ui_existing_room.delete_popup_visible = true;
                            }
                            ImGui::TreePop();
                        }

                        if (pop)
                        {
                            ImGui::PopStyleColor();
                            pop = false;
                        }

                        ii++;
                        ImGui::PopID();
                    }
//                    ImGui::Bullet(); ImGui::Text("Bullet point 3 (two calls)");

                    ImGui::Bullet();

                    ImGui::PushItemWidth(80);
                    ImGui::SameLine();
                    ImGui::InputInt("width", &ui_new_room.width);
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(80);
                    ImGui::SameLine();
                    ImGui::InputInt("height", &ui_new_room.height);
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(160);
                    ImGui::SameLine();
                    if (ImGui::InputTextWithHint("name", "<new room name>", ui_new_room.name.data(), ui_new_room.name.size(), ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                    }
                    ImGui::PopItemWidth();

                    ImGui::SameLine();

                    if (ImGui::SmallButton("+"))
                    {
                        if (!all_rooms.empty()) {
                            save_room_tiles(all_rooms.at(focus_idx));
                        }

                        auto& r = all_rooms.emplace_back(
                                ui_new_room.name,
                                ui_new_room.width,
                                ui_new_room.height
                        );
                        focus_idx = all_rooms.size() - 1;
                        load_room_tiles(r);
                    }
                }

                // TODO: Room group

                ImGui::End();

                if (ui_existing_room.delete_popup_visible)
                {
                    ImGui::SetNextWindowSize(ImVec2(200, 80), ImGuiCond_Always);
                    ImGui::Begin("Really delete?");

                    if (ImGui::Button("Delete"))
                    {
                        all_rooms.erase(all_rooms.begin() + room_to_delete_idx);
                        ui_existing_room.delete_popup_visible = false;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel"))
                    {
                        ui_existing_room.delete_popup_visible = false;
                    }
                    ImGui::End();
                }
            };

            if (!all_rooms.empty())
            {
                load_room_tiles(all_rooms.at(0));
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
        bool _visible = true;
        std::function<void()> _render_console_callback;
        std::function<void()> _dont_render_callback;
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

        RoomGroup group;
        group.name = "nam123";
        group.rooms.emplace_back();
        log_info("%s", cJSON_Print(group.to_json()));

        auto &registry = EntityRegistry::instance().get_registry();

        const auto entity = registry.create();
        auto cheat_console_script = std::make_shared<TileBrowserScript>(entity);
        TileBrowserWindowComponent cheat_console_component{};
        ScriptingComponent scripting_component(cheat_console_script);
        registry.emplace<ScriptingComponent>(entity, scripting_component);

        ImguiComponent imgui_component;
        imgui_component.render_callback = [](){};
        registry.emplace<ImguiComponent>(entity, imgui_component);
        registry.emplace<TileBrowserWindowComponent>(entity, cheat_console_component);
        return entity;
    }
}