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
    boolean widthGet;
    boolean counterHold;

    const byte maxEscapeCounter = 8;
    byte escapeCounter = maxEscapeCounter;
    byte counter = 10;
    byte maxCounter;

    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    const unsigned long secMillis = 1000;
    const unsigned long blinkMillis = 700;
    unsigned long prewCounterMillis;
    unsigned long prewCursorMillis;
    unsigned long prewMoveMillis;
    unsigned long prewEscapeMillis;

public:
    Timer(/* args */);
    ~Timer();

    void updateEscape();
    boolean moveReady();
    boolean blinkReady();
    void startEscape(boolean &blue, boolean &red);
    void escapeMenuTimer();
    void changeTimer(boolean minus, boolean plus);
    void minusCounter();
    void timerReset();
    void readTimer();
    void writeTimer();
};

#endif