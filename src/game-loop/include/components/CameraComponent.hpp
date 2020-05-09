#pragma once

#include <cstdint>

class MainDude;

class CameraComponent
{
public:
    void update(const MainDude&, uint32_t delta_time_ms);
};
