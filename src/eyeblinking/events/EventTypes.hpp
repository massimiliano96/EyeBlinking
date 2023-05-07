#ifndef _EVENT_TYPES_HPP_
#define _EVENT_TYPES_HPP_

#include "Event.hpp"
#include "data/BlinkDetected.hpp"
#include "data/EyeDetected.hpp"
#include "data/FaceDetected.hpp"

class BlinkDetectedEvent : public Event<BlinkDetected>
{
public:
    explicit BlinkDetectedEvent(const BlinkDetected& obj) : Event<BlinkDetected>(obj, "BlinkDetected") {};
    virtual ~BlinkDetectedEvent() = default;
};

class FaceDetectedEvent : public Event<FaceDetected>
{
public:
    explicit FaceDetectedEvent(const FaceDetected& obj) : Event<FaceDetected>(obj, "FaceDetected") {};
    virtual ~FaceDetectedEvent() = default;
};

class EyeDetectedEvent : public Event<EyeDetected>
{
public:
    explicit EyeDetectedEvent(const EyeDetected& obj) : Event<EyeDetected>(obj, "EyeDetected") {};
    virtual ~EyeDetectedEvent() = default;
};

#endif /* _EVENT_TYPES_HPP_ */