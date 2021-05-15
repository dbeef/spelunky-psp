#pragma once

#include "Observer.hpp"
#include "logger/log.h"

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

    void remove_all_observers()
    {
        _observers = {};
    }

    void remove_observer(const Observer<EventType>* observer)
    {
        const auto iter = std::remove_if(_observers.begin(), _observers.end(), [observer](const auto& other){ return observer == other; });
        if (iter != _observers.end())
        {
            _observers.erase(iter);
        }
        else
        {
            log_warn("Failed to remove observer - not subscribed to this subject.");
        }
    }

    void notify(EventType event)
    {
        for (auto& observer : _observers)
        {
            observer->on_notify(&event);
        }
    }

    bool has_observers() const
    {
        return !_observers.empty();
    }

private:

    std::vector<Observer<EventType>*> _observers;
};
