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
    boolean motorSwitch;
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

    void motorState(boolean &motorSwitch, boolean state, Timer &timer, boolean &resetMotor);
    void totalOFF(Buttons &motor, boolean &manualSwitch, Timer &timer);
    void blueButton(Buttons &motor, Buttons &buttonPlus, Timer &timer);
    void redButton(Buttons &buttonMinus, Buttons &motor, Timer &timer);
    void motorCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif
