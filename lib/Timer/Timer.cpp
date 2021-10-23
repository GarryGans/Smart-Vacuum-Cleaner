#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
}

boolean Timer::minusCounter(byte &counter)
{
    if (wait(sec))
    {
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

boolean Timer::ready(byte counter, boolean reset)
{
    static byte count;
    static boolean first;

    if (reset || !first)
    {
        count = counter;
        first = true;
    }

    if (minusCounter(count))
    {
        count = counter;
        first = false;
        return true;
    }

    return false;
}

boolean Timer::wait(unsigned long set)
{
    static unsigned long prew;
    static boolean first;

    if (!first)
    {
        first = true;
        prew = millis();
    }

    if (millis() - prew >= set)
    {
        first = false;
        return true;
    }

    return false;
}

boolean Timer::alternation(unsigned long set)
{
    static boolean blink;

    if (wait(set))
    {
        blink = wait(set * 2);
    }

    return blink;
}
