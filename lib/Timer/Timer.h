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
    boolean move;
    boolean escape;

    byte escapeCounter = 5;
    byte maxEscapeCounter = 5;

    byte counter = 10;
    byte maxCounter;
    byte counterHold;
    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    unsigned long prewCounterMillis;
    const unsigned long secMillis = 1000;
    const unsigned long blinkMillis = 700;
    unsigned long prewCursorMillis;
    unsigned long prewMoveMillis;
    unsigned long prewEscapeMillis;

public:
    Timer(/* args */);
    ~Timer();

    void updateEscape();
    boolean moveReady();
    boolean blinkReady();

    void escapeMenuTimer();
    void changeTimer(boolean minus, boolean plus);
    void minusCounter();
    void timerReset();
    byte getTimer();
    void readTimer();
    void writeTimer();
};

#endif