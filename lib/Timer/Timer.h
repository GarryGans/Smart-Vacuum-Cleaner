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
    byte counter = 10;
    byte maxCounter;
    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    unsigned long prewCounterMillis;
    const unsigned long secMillis = 1000;

public:
    Timer(/* args */);
    ~Timer();

    void changeTimer(boolean minus, boolean plus);
    void minusCounter();
    void timerReset();
    byte getTimer();
    void readTimer();
    void writeTimer();
};

#endif