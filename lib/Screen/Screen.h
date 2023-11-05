#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <EFX.h>

#include <Switchers.h>
#include <Buttons.h>

#define manStrPad 10
#define manStrSpeed 80

#define escCount 4
#define escTime 1000

#define stateStrDeep 0
#define manSpeed 40
#define autoSpeed 80

class Screen : public EFX
{
private:
    boolean esc = false;

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