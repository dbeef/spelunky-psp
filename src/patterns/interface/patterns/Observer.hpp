#pragma once

#include <memory>

template<class EventType>
class Observer
{
public:
    virtual void on_notify(const EventType*) = 0;
    virtual ~Observer() = default;
};
