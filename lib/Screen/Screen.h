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

    const char *vacState[2] = {"OFF", "ON"};

    boolean moveLeft;
    boolean moveRight;
    boolean move;
    boolean restart;

    const byte deep_x = 30;
    byte bottom_x;
    byte bottom_y = 59;

    byte digWidth;
    byte width;
    byte x;
    byte y;
    byte blockWidth;

    enum Position
    {
        center,
        left,
        right
    } position;

public:
    Screen(/* args */);
    ~Screen();

    void showBlink(Timer &timer);
    void logo();
    void align(byte WH, Position position);
    void iconAlign(int icon, byte iconWH, Position position);
    void digAlign(byte dig, const char *string, byte y, Position position, boolean digMix);
    void textAlign(const char *string, byte y, Position position);
    void escapeBar(Timer &timer);
    void showTimerSet(Timer &timer);
    void setTimerScreen(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void blockScreen(Buttons &buttonMinus);
    void bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void moveString(Timer &timer, byte end_x, byte bottom_y, const char *string);
    void showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer);
    void vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif