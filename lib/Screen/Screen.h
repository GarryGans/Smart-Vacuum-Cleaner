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
    const char *textCounter = "s";
    const char *vacState[2] = {"OFF", "ON"};

    Timer timer;

public:
    Screen();
    ~Screen();

    void logo();
    void setTimerScreen(Buttons &buttons);
    void bottomLine(Buttons &buttons);
    void vacuumState(Switchers &relayState, Buttons &buttons);
    void screens(Switchers &relayState, Buttons &buttons);
};

#endif