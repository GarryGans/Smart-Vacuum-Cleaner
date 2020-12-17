#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#include <Switchers.h>
#include <Buttons.h>
#include <Timer.h>

class Screen : public U8G2_SH1106_128X64_NONAME_1_HW_I2C
{
private:
    const String vacState[2] = {"OFF", "ON"};

public:
    Screen(/* args */);
    ~Screen();

    void showBlink(Timer &timer);

    void logo();

    void showTimerSet(Timer &timer);
    void setTimerScreen(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void blockScreen();
    void showAlert();
    void bottomLine(Buttons &buttonMinus, Buttons &buttonPlus);

    void showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer);
    void vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif