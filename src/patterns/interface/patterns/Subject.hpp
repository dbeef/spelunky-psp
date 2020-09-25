#pragma once

#include "Observer.hpp"

#include <vector>
#include <algorithm>

template<class EventType>
class Subject
{
public:

    Subject(std::size_t capacity = 4)
    {
        _observers.reserve(capacity);
    }

    ~Subject() = default;

    void add_observer(Observer<EventType>* observer)
    {
        _observers.emplace_back(observer);
    }

    void remove_observer(const Observer<EventType>* observer)
    {
        const auto iter = std::remove_if(_observers.begin(), _observers.end(), [observer](const auto& other){ return observer == other; });
        _observers.erase(iter);
    }

    void notify(EventType event)
    {
        for (auto& observer : _observers)
        {
            observer->on_notify(&event);
        }
    }

private:

    std::vector<Observer<EventType>*> _observers;
};
