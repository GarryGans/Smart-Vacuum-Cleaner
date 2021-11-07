#include "Switchers.h"

Switchers::Switchers()
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

void Switchers::switcher(Buttons &buttons)
{
    if (buttons.manualSwitch || buttons.pedalSwitch)
    {
        digitalWrite(vacuumPin, ON);
    }
    else 
    {
        digitalWrite(vacuumPin, OFF);
    } 
}