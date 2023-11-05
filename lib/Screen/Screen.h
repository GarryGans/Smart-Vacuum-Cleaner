#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <EFX.h>

#include <Switchers.h>
#include <Buttons.h>

class Screen : public EFX
{
private:
    boolean a = false;

    const char *textCounter = "s";
    const char *vacState[2] = {"OFF", "ON"};

public:
    Screen();
    ~Screen();

    void logo();
    void setTimerScreen(Buttons &buttons);
    void bottomLine(Buttons &buttons);
    void vacuumState(Buttons &buttons);
    void screens(Buttons &buttons);
};

#endif