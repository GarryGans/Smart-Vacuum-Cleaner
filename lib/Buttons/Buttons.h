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
    boolean resetMotor;
    boolean manualSwitch;
    boolean pedalSwitch;
    boolean timerReset;
    boolean minus;
    boolean plus;
    boolean lock;
    boolean unlock;

public:
    Buttons(int8_t pin);
    Buttons();
    ~Buttons();

    void begin();
    void setTimer(boolean blue, boolean red);

    void motorState(boolean &pedalSwitch, boolean state, Timer &timer, boolean &resetMotor);
    void totalOFF(Buttons &pedal, boolean &manualSwitch, Timer &timer);
    void blueButton(Buttons &pedal, Buttons &buttonPlus, Timer &timer);
    void redButton(Buttons &buttonMinus, Buttons &pedal, Timer &timer);
    void pedalCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif
