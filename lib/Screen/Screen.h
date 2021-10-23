#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <EFX.h>

#include <Switchers.h>
#include <Buttons.h>
#include <Timer.h>

class Screen : public EFX
{
private:
    const byte screenWidth = 128;
    const byte screenHeight = 64;

    const char *textCounter = "s";
    const char *vacState[2] = {"OFF", "ON"};

public:
    Screen();
    ~Screen();

    void logo();
    void setTimerScreen(Buttons &pedal);
    void bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void vacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer);
    void screens(Switchers &relayState, Buttons &pedal, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif