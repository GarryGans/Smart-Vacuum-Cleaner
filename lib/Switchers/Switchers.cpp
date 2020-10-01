#include "Switchers.h"

Switchers::Switchers(/* args */)
{
}

Switchers::~Switchers()
{
}

void Switchers::begin(byte vacuumPin)
{
    this->vacuumPin = vacuumPin;
    pinMode(vacuumPin, OUTPUT);
    digitalWrite(vacuumPin, OFF);
}

void Switchers::switcher(Buttons &button, Buttons &motor)
{
    if (button.vacuumState == 1 || motor.motorSwitch)
    {
        digitalWrite(vacuumPin, ON);
    }
    else 
    {
        digitalWrite(vacuumPin, OFF);
    }
    relaySW = digitalRead(vacuumPin);
}