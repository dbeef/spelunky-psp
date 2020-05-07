//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_GAMELOOP_HPP
#define RESOURCE_COMPILER_GAMELOOP_HPP

#include <functional>
#include <memory>
#include <vector>

class GameObject;

class GameLoop
{
public:
    GameLoop();
    std::function<void()>& get();
private:
    std::vector<std::shared_ptr<GameObject>> _game_objects;
    std::function<void()> _loop;
};


#endif //RESOURCE_COMPILER_GAMELOOP_HPP
