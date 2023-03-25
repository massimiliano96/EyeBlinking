#ifndef _EVENT_HPP__
#define _EVENT_HPP__
#include <string>

template <typename T>
class Event{
  protected:

    T _type;
    std::string _name;

  public:
  
    Event() = default;
    Event(T type, const std::string& name = "") : _type(type), _name(name){};
    inline const T type() const { return _type;};
    inline const std::string& getName() const { return _name;};
};

#endif