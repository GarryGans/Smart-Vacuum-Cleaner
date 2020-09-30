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
    enum VacuumState
    {
        swOFF,
        swON
    } vacuumState;

    boolean resetMotor;
    boolean manualSwitch;
    boolean motorSwitch;
    boolean setTimerFlag;
    boolean timerReset;
    boolean minus;
    boolean plus;

public:
    
    Buttons(int8_t pin);
    ~Buttons();

    void begin();
    void setTimer(Buttons &motor, Buttons &buttonPlus, Timer &timer);
    void buttonCommands(Buttons &buttonMinus, Buttons &motor, Timer &timer);
    void motorCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif
