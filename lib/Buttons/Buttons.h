#ifndef BUTTONS_H
#define BUTTONS_H

#define couterAddr 0
#define plusPin 5
#define minusPin 4
#define pedalPin 2

#define manDef 240

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

    byte escapeCounter = 4;
    byte temp;

    boolean setTimerFlag = false;

    boolean block = false;

    boolean manualSwitch = false;
    boolean pedalSwitch = false;

    boolean minus = false;
    boolean plus = false;

    boolean startTimer = false;

    boolean blinkHide = false;

    boolean reset = false;

    byte counter = 0;

    byte manualCounter = manDef;

    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 240;

public:
    Buttons();
    ~Buttons();

    void set(GButton &butt);
    void begin();

    boolean blueB();
    boolean redB();

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
