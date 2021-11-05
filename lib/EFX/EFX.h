#ifndef EFX_H
#define EFX_H

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <Timer.h>
// #include <Vector.h>
#include <ArduinoSTL.h>
#include <vector>

using namespace std;

// class EFX : public U8G2_SSD1306_128X64_NONAME_1_HW_I2C
class EFX : public U8G2_SH1106_128X64_NONAME_1_HW_I2C
{
private:
    Timer timer;

    Timer run;

    unsigned long blinkMil = 500;

    byte screenWidth = getWidth();
    byte screenHeight = getHeight();

    byte borderW;
    byte borderH;

    byte width;
    byte height;

    byte x;
    byte y;

    byte blockWidth;

    boolean escBar;

    byte id = 0;

    int icon;
    int lock = 79;
    int unlock = 68;
    byte WH = 48;

    struct stringPoint
    {
        byte move_x;
        boolean move;
        boolean moveLeft;
        boolean moveRight;
        byte start_x;
        //  byte start_y;
    };

    vector<stringPoint> sp;

    vector<Timer> ti;

    byte setX;
    byte setY;

    const byte escapeCounter = 8;

public:
    enum class PosX
    {
        center,
        left,
        leftSpace,
        leftHalf,
        right,
        rightSpace,
        rightHalf,
        centerFrame,
        rightFrameSide,
        rightFrameHalfSide,
        custom,
        customFrame
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
        downFrameSpace,
        downFrameHalf,
        custom
    } pos_y;

    struct moveStr
    {
        String string;
        PosX pos_x;
        PosY pos_y;
        int speed;

        bool operator==(const moveStr &) const;
    };

    vector<moveStr> strMov;

    EFX();
    ~EFX();

    void customX(byte x);
    void customY(byte y);

    void sameScreen();

    byte nextY(byte num, byte id);

    template <typename type>
    byte getDigWidth(type value);

    void alignSimbols(byte WH, byte H, PosX pos_x, PosY pos_y);
    void frameAlign(byte W, byte H, PosX pos_x, PosY pos_y);
    void iconAlign(int icon, byte iconWH, PosX pos_x, PosY pos_y);
    void digStringAlign(byte dig, const char *string, PosX pos_x, PosY pos_y);
    void strDigAlign(const char *string, byte dig, PosX pos_x, PosY pos_y);
    void strDigAlign(const String string, byte dig, PosX pos_x, PosY pos_y);

    template <typename type>
    void digAlign(type dig, PosX pos_x, PosY pos_y);
    void digAlign(byte dig, PosX pos_x, PosY pos_y);

    void setPosition(const char *format, PosX pos_x, PosY pos_y);
    void setPosition(const String format, PosX pos_x, PosY pos_y);

    void textAlign(const char *string, PosX pos_x, PosY pos_y);

    void stringAlign(String str, PosX pos_x, PosY pos_y);

    void setHeight(const uint8_t *font);

    void mover(byte &move_x, byte deep_x, boolean &moveLeft, boolean &moveRight, byte start_x);
    void moveString(const String string, PosX pos_x, PosY pos_y, int speed = 50);
    void escapeBar(boolean reset);

    void blinkFrame(int value, PosX pos_x, PosY pos_y, boolean tempBlock = 0, boolean dig = 0 );

    void blinkFrame(const char *format, byte digAmount, PosX pos_x, PosY pos_y, boolean tempBlock = false);
};

#endif