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

    const char *textCounter = "s";
    const char *vacState[2] = {"OFF", "ON"};


    boolean moveLeft;
    boolean moveRight;
    boolean move;

    const byte deep_x = 30;
    byte start_x;
    byte move_x;
    byte bottom_y = 59;

    byte digWidth;
    byte width;
    byte x;
    byte y;
    byte setX;
    byte setY;
    byte blockWidth;

    enum class PosX
    {
        center,
        left,
        leftSpace,
        leftHalf,
        right,
        rightSpace,
        rightHalf,
        custom
    } pos_x;

    enum class PosY
    {
        center,
        up,
        upSpace,
        upHalf,
        down,
        downSpace,
        downHalf,
        centerFrame,
        upFrame,
        upFrameHalf,
        downFrame,
        downFrameHalf,
        custom
    } pos_y;

public:
    Screen(/* args */);
    ~Screen();

    void showBlink(Timer &timer);
    void logo();

    byte getDigWidth(byte value);
    void align(byte WH, byte H, PosX pos_x, PosY pos_y);
    void frameAlign(byte W, byte H, PosX pos_x, PosY pos_y);
    void iconAlign(int icon, byte iconWH, PosX pos_x, PosY pos_y);
    void digStringAlign(byte dig, const char *str, PosX pos_x, PosY pos_y);
    void digAlign(byte dig, PosX pos_x, PosY pos_y);
    void textAlign(const char *str, PosX pos_x, PosY pos_y);
    void escapeBar(Timer &timer);
    void showTimerSet(Timer &timer);
    void setTimerScreen(Timer &timer);
    void bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
    void mover(byte deep_x);
    void moveString(Timer &timer, byte end_x, byte bottom_y, const char *string);
    void showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer);
    void vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer);
};

#endif