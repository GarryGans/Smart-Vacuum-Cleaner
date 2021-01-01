#include "Screen.h"

Screen::Screen(/* args */) : U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/U8X8_PIN_NONE)
{
}

Screen::~Screen()
{
}

void Screen::logo()
{
    firstPage();
    do
    {
        setFont(u8g2_font_nokiafc22_tr);
        textAlign("smart", 18, center, custom);
        textAlign("vacuum cleaner", 38, center, custom);

        setFont(u8g2_font_t0_12b_tf);
        textAlign("Garik 2020", 59, center, custom);
    } while (nextPage());
}

void Screen::moveString(Timer &timer, byte deep_x, byte bottom_y, const char *string)
{
    if (!move)
    {
        bottom_x = (screenWidth - getStrWidth(string)) / 2;
        move = true;
        moveLeft = true;
        moveRight = false;
    }

    if (move)
    {
        if (timer.moveReady())
        {
            x = (screenWidth - getStrWidth(string)) / 2;
            deep_x = constrain(deep_x, 0, x);

            if (bottom_x > (x - deep_x) && moveLeft)
            {
                bottom_x--;
                if (bottom_x == x - deep_x)
                {
                    moveRight = true;
                    moveLeft = false;
                }
            }
            else if (bottom_x < (deep_x + x) && moveRight)
            {
                bottom_x++;
                if (bottom_x == deep_x + x)
                {
                    moveRight = false;
                    moveLeft = true;
                }
            }
        }
    }
    setCursor(bottom_x, bottom_y);
    print(string);
}

void Screen::align(byte W, byte H, PositionX position_x, PositionY position_y)
{
    switch (position_x)
    {
    case center:
        x = (screenWidth - W) / 2;
        break;

    case left:
        x = (screenWidth / 2 - W) / 2;
        break;

    case right:
        x = (screenWidth + screenWidth / 2 - W) / 2;
        break;

    default:
        break;
    }

    switch (position_y)
    {
    case centerY:
        y = (screenHeight + H) / 2;
        break;

    case up:

        break;

    case down:

        break;

    case centerFrame:
        y = (screenHeight - H) / 2;
        break;

    case custom:
        break;

    default:
        break;
    }
}

void Screen::digAlign(byte dig, const char *string, byte y, PositionX position_x, PositionY position_y, boolean digMix)
{
    if (digMix)
    {
        if (dig > 9 && dig < 100)
        {
            digWidth = getStrWidth("W") * 2 + getStrWidth(string);
        }
        else
        {
            digWidth = getStrWidth("W") + getStrWidth(string);
        }
    }

    else
    {
        if (dig > 9 && dig < 100)
        {
            digWidth = getStrWidth(string) * 2;
        }
        else
        {
            digWidth = getStrWidth(string);
        }
    }
    // Serial.println(getMaxCharWidth());
    align(digWidth, getMaxCharWidth(), position_x, position_y);
    setCursor(x, y);

    print(dig);
    if (digMix)
    {
        print(string);
    }
}

void Screen::textAlign(const char *string, byte y, PositionX position_x, PositionY position_y)
{
    align(getStrWidth(string), 0, position_x, position_y);
    setCursor(x, y);
    print(string);
}

void Screen::frameAlign(byte W, byte H, PositionX position_x, PositionY position_y)
{
    align(W, H, position_x, position_y);
    drawFrame(x, y, W, H);
}

void Screen::escapeBar(Timer &timer)
{
    if (!timer.widthGet)
    {
        blockWidth = screenWidth / timer.maxEscapeCounter;
        timer.widthGet = true;
    }

    width = blockWidth * (timer.maxEscapeCounter - timer.escapeCounter);
    drawBox(0, 58, width, 6);

    if (width == blockWidth * timer.maxEscapeCounter)
    {
        timer.widthGet = false;
    }
}

void Screen::showBlink(Timer &timer)
{
    if (timer.blinkReady() && !timer.blinkHold)
    {

        frameAlign(digWidth + 10, getMaxCharWidth() + 10, center, centerFrame);
        digAlign(timer.counter, "W", 40, center, custom, false);
        // Serial.println(getMaxCharWidth());
    }
    else
    {
        digAlign(timer.counter, "W", 40, center, custom, false);
    }
}

void Screen::showTimerSet(Timer &timer)
{
    setFont(u8g2_font_courB18_tr);
    showBlink(timer);

    escapeBar(timer);
}

void Screen::setTimerScreen(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (buttonMinus.setTimerFlag || buttonPlus.setTimerFlag)
    {
        firstPage();
        do
        {
            showTimerSet(timer);
        } while (nextPage());
    }
}

void Screen::iconAlign(int icon, byte iconWH, PositionX position_x, PositionY position_y)
{
    align(iconWH, iconWH, position_x, position_y);
    drawGlyph(x, y, icon);
}

void Screen::blockScreen(Buttons &buttonMinus)
{
    setFont(u8g2_font_open_iconic_thing_6x_t);
    if (buttonMinus.buttonLock)
    {
        iconAlign(79, 48, center, centerY);
    }
    if (buttonMinus.unlock)
    {
        iconAlign(68, 48, center, centerY);
    }
}

void Screen::bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    setFont(u8g2_font_pixelmordred_tf);

    if (buttonMinus.isClick() || buttonMinus.isHold() || buttonPlus.isClick() || buttonPlus.isHold())
    {
        move = false;
    }

    if (buttonPlus.manualSwitch)
    {
        moveString(timer, deep_x, bottom_y, "manual mode");
    }
}

void Screen::showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer)
{
    if (relayState.relaySW && !buttonPlus.manualSwitch)
    {
        setFont(u8g2_font_HelvetiPixelOutline_tr);
        if (relayState.relaySW)
        {
            textAlign(vacState[1], 40, left, custom);
        }
        else
        {
            textAlign(vacState[0], 40, left, custom);
        }

        setFont(u8g2_font_courB18_tr);
        digAlign(timer.counter, textCounter, 40, right, custom, true);
    }

    else
    {
        setFont(u8g2_font_HelvetiPixelOutline_tr);
        if (relayState.relaySW)
        {
            textAlign(vacState[1], 40, center, custom);
        }
        else
        {
            textAlign(vacState[0], 40, center, custom);
        }
    }
}

void Screen::vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.setTimerFlag && !buttonPlus.setTimerFlag && !timer.widthGet)
    {
        firstPage();
        do
        {
            if (buttonMinus.buttonLock)
            {
                blockScreen(buttonMinus);
            }
            else
            {
                showVacuumState(relayState, buttonPlus, timer);
            }

            bottomLine(buttonMinus, buttonPlus, timer);
        } while (nextPage());
    }
}