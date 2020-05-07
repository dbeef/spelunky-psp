#pragma once

class GameObject
{
public:
    virtual ~GameObject() = default;
    virtual void update() = 0;
    bool is_marked_for_disposal() const { return _to_dispose; }
private:
    bool _to_dispose = false;
};
