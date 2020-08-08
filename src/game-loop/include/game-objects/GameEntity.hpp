#pragma once

#include <cstdint>

class GameEntity
{
public:
    virtual ~GameEntity() = default;
    virtual void update(uint32_t delta_time_ms) = 0;
    bool is_marked_for_disposal() const { return _to_dispose; }
protected:
    void mark_for_disposal() { _to_dispose = true; }
    bool _to_dispose = false;
};
