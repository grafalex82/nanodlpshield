#pragma once

#include <wiringPi.h>

class Pin
{
    int _pin;
    bool _invert;

public:
    Pin(int pin, int mode)
        : _pin(pin)
    {
        pinMode(_pin, mode);
    }

    Pin(int pin, int mode, int pu_mode, bool invert = false)
        : _pin(pin)
        , _invert(invert)
    {
        pinMode(_pin, mode);
        pullUpDnControl(_pin, pu_mode);
    }

    void setInvertMode(bool invert)
    {
        _invert = invert;
    }

    bool read()
    {
        return digitalRead(_pin) ^ _invert;
    }

    bool write(bool val)
    {
        digitalWrite(_pin, val);
    }
};
