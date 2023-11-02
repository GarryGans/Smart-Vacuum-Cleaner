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
    Timer timer[4];

    byte escapeCounter = 3;

    boolean setTimerFlag;

    boolean block;
    boolean manualSwitch;
    boolean pedalSwitch;
    boolean minus;
    boolean plus;

    boolean startTimer;

    boolean blinkHide;

    boolean reset;

    byte counter;

    byte manualCounter = 10;

    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 255;

public:
    Buttons();
    ~Buttons();

    void set(GButton &butt);
    void begin();

    void setTimer();

    void buttons();
    void pedalCommands();

    void manualMode();

    boolean changeTimer(boolean minus, boolean plus);

    void resetTimer();
    void resetSet();

    void readTimer();
    void writeTimer();

    void com();
};

#endif
