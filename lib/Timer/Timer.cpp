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
    // Serial.print("write counter: ");
    // Serial.println(counter);
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
    // Serial.println(counter);
}

void Timer::minusCounter()
{   
    if ((millis() - prewCounterMillis >= secMillis))
    {
        EEPROM.get(couterAddr, maxCounter);
        prewCounterMillis = millis();
        counter--;
        counter = constrain(counter, 0, maxCounter);
        // Serial.println(counter);
        // Serial.println(maxCounter);
    }
}

void Timer::timerReset()
{
    EEPROM.get(couterAddr, counter);
    // Serial.print("reset counter ");
    // Serial.println(counter);
}