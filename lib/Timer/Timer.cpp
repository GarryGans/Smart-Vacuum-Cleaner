#include "Timer.h"

Timer::Timer(/* args */)
{
}

Timer::~Timer()
{
}

byte Timer::getTimer()
{
    return counter;
}

void Timer::readTimer()
{
    EEPROM.get(couterAddr, counter);
    if (counter == 255)
    {
        counter = defaultCounter;
    }
}

void Timer::writeTimer()
{
    EEPROM.put(couterAddr, counter);
}

boolean Timer::moveReady()
{
    if (millis() - prewMoveMillis >= secMillis / 4)
    {
        move = false;
        if (millis() - prewMoveMillis >= secMillis / 2)
        {
            prewMoveMillis = millis();
            move = true;
        }
    }

    return move;
}

boolean Timer::blinkReady()
{
    if (millis() - prewCursorMillis >= blinkMillis)
    {
        blink = true;
        if (millis() - prewCursorMillis >= blinkMillis * 2)
        {
            prewCursorMillis = millis();
            blink = false;
        }
    }

    return blink;
}

void Timer::escapeMenuTimer()
{
    if (millis() - prewEscapeMillis >= blinkMillis)
    {
        prewEscapeMillis = millis();
        if (escapeCounter > 0)
        {
            escapeCounter--;
        }
    }
}

void Timer::updateEscape()
{
    escapeCounter = maxEscapeCounter;
}

void Timer::changeTimer(boolean minus, boolean plus)
{
    if (minus)
    {
        counter--;
        counter = constrain(counter, minSetCounter, maxSetCounter);
    }
    else if (plus)
    {
        counter++;
        counter = constrain(counter, minSetCounter, maxSetCounter);
    }
}

void Timer::minusCounter()
{
    if ((millis() - prewCounterMillis >= secMillis))
    {
        EEPROM.get(couterAddr, maxCounter);
        prewCounterMillis = millis();
        if (counter > 0)
        {
            counter--;
        }
    }
}

void Timer::timerReset()
{
    EEPROM.get(couterAddr, counter);
}