#ifndef DISPATCHER_HH__
#define DISPATCHER_HH__

#include "Event.hpp"

#include <functional>
#include <vector>
#include <algorithm>

/**
  @class Dispatcher
  @brief Dispatches events to observers
  The dispatcher is the central class for event management. It offers clients
  the opportunity to use post() to submit a given event. Said event is then
  passed on to all observers that are listening to it. The dispatcher also
  handles new observers via subscribe() and permits the removal of observers
  via disconnect().
*/
template<typename T>
class Dispatcher{

  private:
    using SlotType = std::function<void(const Event<T>&)>;
    //std::map<T, std::vector<SlotType>> _observers;
    std::vector<SlotType> _observers;  //vertor of observers for the event T
  public:

    /**
     * Subscribe an observer
     * @param type  The type of the function to listen to.
     * @param funct function of the observer
     */
    void subscribe(const SlotType& funct){
      _observers.push_back(funct);
    };

    /**
     * Send the event to all the observers
     * @param event event to post
     */
    void post(Event<T>& event){
      if(_observers.size() == 0)
        return;

      //Loop though all observers. If the event is not handled yet we continue to process it.
      for(auto&& observer : _observers){    
        observer(event);  
      }
    };

};

#endif