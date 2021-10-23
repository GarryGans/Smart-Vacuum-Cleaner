#ifndef BUTTONS_H
#define BUTTONS_H

#define couterAddr 0

#include <Arduino.h>
#include "GyverButton.h"
#include "Timer.h"
#include <EEPROM.h>

class Buttons : public GButton
{
    friend class Switchers;
    friend class Screen;

private:
    Timer timer;
    const unsigned long secMillis = 1000;

    const byte maxEscapeCounter = 5;
    byte escapeCounter = maxEscapeCounter;

    boolean setTimerFlag;

    boolean escBar;

    boolean manualSwitch;
    boolean pedalSwitch;
    boolean minus;
    boolean plus;

    boolean startTimer;

    enum Operator
    {
        decrease,
        increase
    };

    boolean blinkHide;

    byte counter;

    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    

public:
    Buttons(int8_t pin);
    ~Buttons();

    void maxEscape();

    void readTimer();
    void writeTimer();

    void begin();
    void setTimer(boolean &manualSwitch, Timer &timer, Buttons &pedal, Operator state);
    void blueButton(Buttons &buttonPlus, Buttons &pedal, Timer &timer);
    void redButton(Buttons &buttonMinus, Buttons &pedal, Timer &timer);
    void pedalCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);

    void startEscSet();
    void changeTimer(boolean minus, boolean plus);

    void resetTimer();

    void resetEscape();
};

#endif
