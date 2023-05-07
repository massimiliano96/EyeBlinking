#ifndef _BLINK_DETECTED_HPP_
#define _BLINK_DETECTED_HPP_

#include <string>

class BlinkDetected
{
public:
    BlinkDetected() : name("BlinkDetected") {};

    ~BlinkDetected() = default;

    std::string getName() const;

private:
    std::string name = "";
};

#endif /*  __BLINK_DETECTED_HPP__ */
