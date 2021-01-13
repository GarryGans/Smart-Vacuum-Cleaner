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
    boolean escBar;
    boolean blinkHide;
    boolean setTimerFlag;

    const byte maxEscapeCounter = 5;
    byte escapeCounter = maxEscapeCounter;
    const byte maxUnblockCounter = 2;
    byte unblockCounter = maxUnblockCounter;
    

    const byte defaultCounter = 10;
    const byte minSetCounter = 1;
    const byte maxSetCounter = 99;

    const unsigned long secMillis = 1000;
    const unsigned long blinkMillis = 200;
    unsigned long prewCounterMillis;
    unsigned long prewCursorMillis;
    unsigned long prewMoveMillis;
    unsigned long prewEscapeMillis;

public:
    byte counter = 10;

    Timer(/* args */);
    ~Timer();

    void maxEscape();
    void maxUnblock();
    boolean reduceTimer(byte &counter);

    boolean moveReady();
    boolean blinkReady();
    void resetEscape();

    void startEscSet();
    void changeTimer(boolean minus, boolean plus);
    void readTimer();
    void writeTimer();
};

#endif