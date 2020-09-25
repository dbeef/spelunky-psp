#pragma once

#include <random>

class Spawner
{
public:
    Spawner(int chance_of_spawning_percent, int max_number_of_spawned)
        : _chance_of_spawning_percent(chance_of_spawning_percent)
        , _max_number_of_spawned(max_number_of_spawned)
    {}

    void spawned() { _number_spawned++; }

    bool can_spawn() const
    {
        if (_number_spawned >= _max_number_of_spawned)
        {
            return false;
        }

        static std::random_device random_device;
        std::default_random_engine engine(random_device());
        std::uniform_int_distribution<int> uniform_dist(0, 100);
        return uniform_dist(engine) < _chance_of_spawning_percent;
    }

private:
    const int _chance_of_spawning_percent;
    const int _max_number_of_spawned;
    int _number_spawned = 0;

};
