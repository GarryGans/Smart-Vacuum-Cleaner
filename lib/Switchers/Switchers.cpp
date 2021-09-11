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

void Switchers::switcher(Buttons &button, Buttons &pedal)
{
    if (button.manualSwitch || pedal.pedalSwitch)
    {
        digitalWrite(vacuumPin, ON);
        relaySW = true;
    }
    else 
    {
        digitalWrite(vacuumPin, OFF);
        relaySW = false;
    } 
}