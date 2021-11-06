#ifndef BUTTONS_H
#define BUTTONS_H

#define couterAddr 0
#define plusPin 5
#define minusPin 4
#define pedalPin 2

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

    const byte escapeCounter = 8;

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

    Buttons();
    ~Buttons();

    void set(GButton &butt);
    void begin();
    void setTimer(boolean &manualSwitch, Timer &timer, Operator state);
    void blueButton(Timer &timer);
    void redButton(Timer &timer);
    void pedalCommands(Timer &timer);

    void changeTimer(boolean minus, boolean plus);

    void escSet();
    void resetTimer();
    void maxEscape();

    void readTimer();
    void writeTimer();
};

#endif
