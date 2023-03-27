#ifndef __BLINK_DETECTED_HPP__
#define __BLINK_DETECTED_HPP__

#include <string>

class BlinkDetected 
{
    public:

    BlinkDetected ()
    {
        name = "BlinkDetected";
    };

    ~BlinkDetected() = default;

    std::string getName();

    private:
    std::string name = "";
};

#endif /*  __BLINK_DETECTED_HPP__ */
