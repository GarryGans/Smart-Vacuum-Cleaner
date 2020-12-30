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
        textAlign("smart", 18, center);
        textAlign("vacuum cleaner", 38, center);

        setFont(u8g2_font_t0_12b_tf);
        textAlign("Garik 2020", 59, center);
    } while (nextPage());
}

void Screen::moveString(Timer &timer, byte deep_x, byte bottom_y, const char *string)
{
    if (!difGet)
    {
        bottom_x = (screenWidth - getStrWidth(string)) / 2;
        difGet = true;
        moveLeft = true;
        moveRight = false;
    }

    if (difGet)
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

void Screen::digAlign(byte dig, const char *string, byte y, Position position, boolean digMix)
{
    if (digMix)
    {
        if (dig > 9)
        {
            digWidth = getStrWidth(string) * 3;
        }
        else
        {
            digWidth = getStrWidth(string) * 2;
        }
    }

    else
    {
        if (dig > 9)
        {
            digWidth = getStrWidth(string) * 2;
        }
        else
        {
            digWidth = getStrWidth(string);
        }
    }

    switch (position)
    {
    case center:
        x = (screenWidth - digWidth) / 2;
        break;

    case left:
        x = (screenWidth / 2 - digWidth) / 2;
        break;

    case right:
        x = (screenWidth + (screenWidth / 2) - digWidth) / 2;
        break;

    default:
        break;
    }

    setCursor(x, y);
    print(dig);
    if (digMix)
    {
        print(string);
    }
}

void Screen::textAlign(const char *string, byte y, Position position)
{
    switch (position)
    {
    case center:
        x = (screenWidth - getStrWidth(string)) / 2;
        break;
    case left:
        x = (screenWidth / 2 - getStrWidth(string)) / 2;
        break;

    case right:
        x = (screenWidth + (screenWidth / 2) - getStrWidth(string)) / 2;
        break;

    default:
        break;
    }

    setCursor(x, y);
    print(string);
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
        if (timer.counter > 9)
        {
            textAlign("__", 40, left);
        }
        else
        {
            textAlign("_", 40, left);
        }
    }
    else
    {
        digAlign(timer.counter, "0", 40, left, false);
    }
}

void Screen::showTimerSet(Timer &timer)
{
    setFont(u8g2_font_courB18_tr);
    showBlink(timer);

    setFont(u8g2_font_open_iconic_app_4x_t);
    iconAlign(69, 32, right);

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

void Screen::iconAlign(int icon, byte iconWH, Position position)
{
    switch (position)
    {
    case center:
        x = (screenWidth - iconWH) / 2;
        y = (screenHeight + iconWH) / 2;
        break;

    case left:
        x = (screenWidth / 2 - iconWH) / 2;
        y = (screenHeight + iconWH) / 2;
        break;

    case right:
        x = screenWidth / 2 + (screenWidth / 2 - iconWH) / 2;
        y = (screenHeight + iconWH) / 2;
        break;

    default:
        break;
    }

    drawGlyph(x, y, icon);
}

void Screen::blockScreen(Buttons &buttonMinus)
{
    setFont(u8g2_font_open_iconic_thing_6x_t);
    if (buttonMinus.buttonLock)
    {
        iconAlign(79, 48, center);
    }
    if (buttonMinus.unlock)
    {
        iconAlign(68, 48, center);
    }
}

void Screen::bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    setFont(u8g2_font_pixelmordred_tf);

    if (buttonMinus.isClick() || buttonMinus.isHold() || buttonPlus.isClick() || buttonPlus.isHold())
    {
        difGet = false;
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
            textAlign(vacState[1], 40, left);
        }
        else
        {
            textAlign(vacState[0], 40, left);
        }

        setFont(u8g2_font_courB18_tr);
        digAlign(timer.counter, "s", 40, right, true);
    }

    else
    {
        setFont(u8g2_font_HelvetiPixelOutline_tr);
        if (relayState.relaySW)
        {
            textAlign(vacState[1], 40, center);
        }
        else
        {
            textAlign(vacState[0], 40, center);
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