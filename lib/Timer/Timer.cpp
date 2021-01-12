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

void Timer::maxUnblock()
{
    unblockCounter = maxUnblockCounter;
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

void Timer::writeTimer()
{
    EEPROM.put(couterAddr, counter);
}

void Timer::maxEscape()
{
    escapeCounter = maxEscapeCounter;
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

void Timer::startEscape()
{
    if (!blinkHide)
    {
        if (reduceTimer(escapeCounter) && !escBar)
        {
            resetEscape();
        }
    }
}

boolean Timer::reduceTimer(byte &counter)
{
    if (millis() - prewCounterMillis >= secMillis)
    {
        prewCounterMillis = millis();
        if (counter > 0)
        {
            counter--;
        }
    }

    if (counter == 0)
    {
        return true;
    }

    return false;
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
