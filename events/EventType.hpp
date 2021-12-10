#ifndef _EVENT_TYPE_HPP_
#define _EVENT_TYPE_HPP_

#include "Event.hpp"
#include "BlinkDetected.hpp"
#include "FaceDetected.hpp"

class BlinkDetectedEvent : public Event<BlinkDetected>
{
public:
  BlinkDetectedEvent(BlinkDetected& obj) : Event<BlinkDetected>(obj, "BlinkDetected"){};
  virtual ~BlinkDetectedEvent() = default;
};

class FaceDetectedEvent : public Event<FaceDetected>
{
public:
  FaceDetectedEvent(FaceDetected& obj) : Event<FaceDetected>(obj, "FaceDetected"){};
  virtual ~FaceDetectedEvent() = default;
};

#endif /* _EVENT_TYPE_HPP_ */