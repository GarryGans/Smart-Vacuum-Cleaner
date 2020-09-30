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
    boolean blink;
    const unsigned long blinkMillis = 700;
    unsigned long prewCursorMillis;

    const String vacState[2] = {"OFF", "ON"};

public:
    Screen(/* args */);
    ~Screen();

    boolean blinkReady();
    void showBlink(int value);

    // void highLighter(Buttons &buttons);

    void logo();

    void showTimerSet(Timer &timer);
    void setTimerScreen(Buttons &buttonMinus, Timer &timer);

    void showAlert();
    void bottomLine(Buttons &buttonPlus);
    void showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer);
    void vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif