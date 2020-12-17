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

void Timer::escapeTimer()
{  
    if (millis() - prewEscapeMillis >= blinkMillis)
    {
        prewEscapeMillis = millis();
        escapeCounter--;
        escapeCounter = constrain(escapeCounter, 0 , defaultCounter);
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
        counter--;
        counter = constrain(counter, 0, maxCounter);
    }
}

void Timer::timerReset()
{
    EEPROM.get(couterAddr, counter);
}