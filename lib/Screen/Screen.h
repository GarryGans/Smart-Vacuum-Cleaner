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
    const byte screenWidth = 128;
    const byte screenHeight = 64;

    const String vacState[2] = {"OFF", "ON"};
    boolean moveLeft;
    boolean moveRight;
    boolean difGet;
    boolean restart;
    boolean widthGet;

    byte allert_x = 18;
    byte auto_x = 24;
    byte block_x = 20;
    const byte deep_x = 30;
    byte bottom_x;
    byte bottom_y = 59;

    byte blockWidth;

public:
    Screen(/* args */);
    ~Screen();

    void showBlink(Timer &timer);

    void logo();

    void textAlign(const char *string, byte y);
    void showTimerSet(Timer &timer);
    void setTimerScreen(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void blockScreen();
    void bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void escapeBar(Timer &timer);
    void moveString(Timer &timer, byte x, byte end_x, const char *string);
    void showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer);
    void vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif