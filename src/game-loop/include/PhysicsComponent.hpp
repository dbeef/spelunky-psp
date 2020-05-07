#pragma once

class MainDude;

class PhysicsComponent
{
public:
    void update(MainDude&);
private:
    float velocity_x = 0;
    float velocity_y = 0;
};
