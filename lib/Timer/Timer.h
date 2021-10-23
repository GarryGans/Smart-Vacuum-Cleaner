#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer
{
    friend class Screen;
    friend class EFX;

private:
    unsigned long sec = 1000;

    unsigned long display = 3000;

    unsigned int autoEsc = 5000;

    const byte maxEscapeCounter = 5;
    byte escapeCounter = maxEscapeCounter;

    boolean escBar;
    boolean move;

public:
    Timer();
    ~Timer();

    boolean minusCounter(byte &counter);
    boolean wait(unsigned long set);

    boolean alternation(unsigned long set = 500);

    boolean ready(byte counter = 5, boolean reset = false);
};

#endif