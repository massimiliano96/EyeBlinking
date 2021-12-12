#ifndef EVENT_HH__
#define EVENT_HH__
#include <string>

/*
  @class Event
  @brief Base class for all events
  This class provides the interface that needs to be implemented by an event.
  Each dependent class is required to implement type() to uniquely identify
  events. 
*/

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