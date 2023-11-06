#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "GyverButton.h"
#include "Timer.h"
#include <EEPROM.h>

#define couterAddr 0
#define plusPin 5
#define minusPin 4
#define pedalPin 2

#define manDef 90

#define defaultCounter 10
#define minSetCounter 1
#define maxSetCounter 240

#define escCount 4

class Buttons : public GButton
{
    friend class Switchers;
    friend class Screen;

private:
    Timer timer[4];

    byte temp;

    boolean setTimerFlag = false;

    boolean block = false;

    boolean manualSwitch = false;
    boolean pedalSwitch = false;

    boolean startTimer = false;

    boolean blinkHide = false;

    boolean reset_A = false;
    boolean reset_M = false;
    boolean reset_B = false;

    // boolean reset_P = false;

    byte counter = 0;

    byte manualCounter = 0;

    byte autoCounter = 0;

public:
    Buttons();
    ~Buttons();

    void set(GButton &butt);
    void begin();

    boolean blueB();
    boolean redB();

    void writeTimer();

    boolean changeTimer(boolean minus, boolean plus);
    void setTimer();

    void manualMode();

    void pedalMode();

    void com();
};

#endif
