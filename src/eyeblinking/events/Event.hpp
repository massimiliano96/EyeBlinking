#ifndef _EVENT_HPP__
#define _EVENT_HPP__
#include <string>

template <typename T>
class Event
{
public:
    Event() = default;
    Event(T type, const std::string& name = "") : eventType(type), name(name) {};

    inline T type() const
    {
        return eventType;
    };

    inline const std::string& getName() const
    {
        return name;
    };

private:
    T eventType;
    std::string name;
};

#endif