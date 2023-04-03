#ifndef DISPATCHER_HH__
#define DISPATCHER_HH__

#include "Event.hpp"

#include <functional>
#include <vector>
#include <algorithm>

/*
  The dispatcher is the central class for event management. It offers clients
  the opportunity to use post() to submit a given event. Said event is then
  passed on to all observers that are listening to it. The dispatcher also
  handles new observers via subscribe().
*/
template<typename T>
class Dispatcher{

  private:
    using SlotType = std::function<void(const Event<T>&)>;
    
    std::vector<SlotType> _observers;  //vertor of observers for the event T
  public:

    /*
    Subscribe an observer
    - type :  The type of the function to listen to.
    - funct : function of the observer
     */
    void subscribe(const SlotType& funct){
      _observers.push_back(funct);
    };

    
    // Send the event to all the observers
    void post(Event<T>& event){
      if(_observers.size() == 0)
        return;

      //Loop though all observers.
      for(auto&& observer : _observers){    
        observer(event);  
      }
    };

    void disconnect()
    {
      _observers.resize(0);
    }


};

#endif