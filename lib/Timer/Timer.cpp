#include "Timer.h"

Timer::Timer(/* args */)
{
}

Timer::~Timer()
{
}

byte Timer::getTimer()
{
    EEPROM.get(couterAddr, counter);
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
    if (millis() - prewMoveMillis >= secMillis / 10)
    {
        move = true;
        if (millis() - prewMoveMillis >= secMillis / 5)
        {
            prewMoveMillis = millis();
            move = false;
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

void Timer::startEscape(boolean &blue, boolean &red)
{
    escapeMenuTimer();
    if (escapeCounter == 0 && !widthGet)
    {
        writeTimer();
        blue = false;
        red = false;
    }
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