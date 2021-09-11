#include "Timer.h"

Timer::Timer(/* args */)
{
}

Timer::~Timer()
{
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
    move = false;

    if (millis() - prewMoveMillis >= secMillis / 20)
    {
        prewMoveMillis = millis();
        move = true;
    }

    return move;
}

boolean Timer::blinkReady()
{
    if (blinkHide)
    {
        blink = false;
    }

    else
    {
        if (millis() - prewCursorMillis >= blinkMillis)
        {
            blink = false;

            if (millis() - prewCursorMillis >= blinkMillis * 2)
            {
                prewCursorMillis = millis();
                blink = true;
            }
        }
    }

    return blink;
}

void Timer::maxEscape()
{
    escapeCounter = maxEscapeCounter;
}

void Timer::maxUnblock()
{
    unblockCounter = maxUnblockCounter;
}

void Timer::resetEscape()
{
    if (setTimerFlag)
    {
        setTimerFlag = false;
        escBar = false;
        maxEscape();
        writeTimer();
    }
}

void Timer::resetTimer()
{
    startTimer = false;
    readTimer();
}

boolean Timer::reduceTimer()
{
    if (millis() - prewCounterMillis >= secMillis)
    {
        prewCounterMillis = millis();
        if (escapeCounter > 0)
        {
            escapeCounter--;
        }
    }

    if (escapeCounter == 0)
    {
        return true;
    }

    return false;
}

void Timer::startEscSet()
{
    if (!blinkHide)
    {
        if (reduceTimer() && !escBar)
        {
            resetEscape();
        }
    }
}

void Timer::changeTimer(boolean minus, boolean plus)
{
    blinkHide = true;
    maxEscape();

    if (minus)
    {
        counter--;
    }

    else if (plus)
    {
        counter++;
    }

    counter = constrain(counter, minSetCounter, maxSetCounter);
}
