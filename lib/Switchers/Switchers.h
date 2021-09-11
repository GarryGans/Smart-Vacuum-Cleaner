#ifndef SWITCHERS_H
#define SWITCHERS_H

#include <Arduino.h>
#include <Buttons.h>


#define OFF LOW
#define ON HIGH

class Switchers
{
    friend class Screen;

private:
    byte vacuumPin;
    boolean relaySW;

public:
    Switchers();
    ~Switchers();

    void begin(byte vacuumPin);
    void switcher(Buttons &button, Buttons &pedal);
};

#endif