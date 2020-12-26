//#pragma once
//
//#include <vector>
//#include <memory>
//#include <utility>
//
//#include "Level.hpp"
//#include "main-dude/MainDude.hpp"
//
//class World
//{
//public:
//
//    World()
//    {
//        // TODO: Create level instead of singleton
//        Level::instance().get_tile_batch().generate_cave_background();
//        Level::instance().get_tile_batch().batch_vertices();
//        Level::instance().get_tile_batch().add_render_entity();
//
//        _main_dude = std::make_shared<MainDude>(0, 0);
//    }
//
//    std::shared_ptr<Level>& get_level() { return _level; }
//    std::shared_ptr<MainDude>& get_main_dude() { return _main_dude; }
//    std::vector<std::shared_ptr<GameEntity>>& get_game_entities() { return _game_entities; }
//
//private:
//    std::shared_ptr<Level> _level;
//    std::shared_ptr<MainDude> _main_dude;
//    std::vector<std::shared_ptr<GameEntity>> _game_entities;
//};
