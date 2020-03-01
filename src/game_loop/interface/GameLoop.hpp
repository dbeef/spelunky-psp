//
// Created by dbeef on 2/5/20.
//

#ifndef RESOURCE_COMPILER_GAMELOOP_HPP
#define RESOURCE_COMPILER_GAMELOOP_HPP

#include <functional>

class GameLoop
{
public:
    GameLoop();
    std::function<void()>& get();
private:
    std::function<void()> _loop;
};


#endif //RESOURCE_COMPILER_GAMELOOP_HPP
