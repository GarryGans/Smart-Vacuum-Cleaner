#ifndef TIMER_H
#define TIMER_H

#define couterAddr 0

#include <Arduino.h>
#include <EEPROM.h>

class Timer
{
    friend class Screen;
    friend class Buttons;
private:
    boolean blink;

    byte counter = 10;
    byte maxCounter;
    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    unsigned long prewCounterMillis;
    const unsigned long secMillis = 1000;
    const unsigned long blinkMillis = 700;
    unsigned long prewCursorMillis;

public:
    Timer(/* args */);
    ~Timer();

    boolean blinkReady();
    void changeTimer(boolean minus, boolean plus);
    void minusCounter();
    void timerReset();
    byte getTimer();
    void readTimer();
    void writeTimer();
};

#endif