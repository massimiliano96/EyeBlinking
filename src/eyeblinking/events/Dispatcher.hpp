#ifndef _DISPATCHER_HPP_
#define _DISPATCHER_HPP_

#include "Event.hpp"

#include <algorithm>
#include <functional>
#include <vector>

template <typename T>
class Dispatcher
{

private:
    using SlotType = std::function<void(const Event<T>&)>;

    std::vector<SlotType> observers;

public:
    void subscribe(const SlotType& funct)
    {
        observers.push_back(funct);
    };

    const std::vector<SlotType> getObservers() const
    {
        return observers;
    };

    void post(Event<T>& event)
    {
        if (observers.size() == 0)
            return;

        for (auto&& observer : observers)
        {
            observer(event);
        }
    };

    void disconnect()
    {
        observers.resize(0);
    }
};

#endif