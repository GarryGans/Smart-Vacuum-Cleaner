#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "GyverButton.h"
#include "Timer.h"

class Buttons : public GButton
{
    friend class Switchers;
    friend class Screen;

private:
    boolean manualSwitch;
    boolean pedalSwitch;
    boolean minus;
    boolean plus;

    enum Operator
    {
        decrease,
        increase
    };

public:
    Buttons(int8_t pin);
    Buttons();
    ~Buttons();

    void begin();
    void motorState(boolean &pedalSwitch, boolean state, Timer &timer);
    void setTimer(boolean &manualSwitch, Timer &timer, Buttons &pedal, Operator state);
    void blueButton(Buttons &buttonPlus, Buttons &pedal, Timer &timer);
    void redButton(Buttons &buttonMinus, Buttons &pedal, Timer &timer);
    void pedalCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif
