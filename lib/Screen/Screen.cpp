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
        textAlign("smart", centerX, up);

        textAlign("vacuum cleaner", centerX, centerY);

        setFont(u8g2_font_t0_12b_tf);
        textAlign("Garik 2020", centerX, down);
    } while (nextPage());
}

void Screen::moveString(Timer &timer, byte deep_x, byte bottom_y, const char *string)
{
    if (!move)
    {
        move_x = (screenWidth - getStrWidth(string)) / 2;
        move = true;
        moveLeft = true;
        moveRight = false;
    }

    setCursor(move_x, bottom_y);
    print(string);

    if (move)
    {
        if (timer.moveReady())
        {
            start_x = (screenWidth - getStrWidth(string)) / 2;
            deep_x = constrain(deep_x, 0, start_x);

            if (move_x > (start_x - deep_x) && moveLeft)
            {
                move_x--;
                if (move_x == start_x - deep_x)
                {
                    moveRight = true;
                    moveLeft = false;
                }
            }
            else if (move_x < (deep_x + start_x) && moveRight)
            {
                move_x++;
                if (move_x == deep_x + start_x)
                {
                    moveRight = false;
                    moveLeft = true;
                }
            }
        }
    }
}

void Screen::align(byte W, byte H, PositionX position_x, PositionY position_y)
{
    switch (position_x)
    {
    case centerX:
        x = (screenWidth - W) / 2;
        break;

    case left:
        x = (screenWidth / 2 - W) / 2;
        break;

    case right:
        x = (screenWidth + screenWidth / 2 - W) / 2;
        break;

    case customX:
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
        y = (screenHeight / 2 - H) / 2 + H;
        break;

    case down:
        y = (screenHeight + screenHeight / 2 - H) / 2 + H;
        break;

    case upFrame:
        y = (screenHeight / 2 - H) / 2;
        break;

    case downFrame:
        y = (screenHeight + screenHeight / 2 - H) / 2;
        break;

    case centerFrame:
        y = (screenHeight - H) / 2;
        break;

    case customY:
        break;

    default:
        break;
    }
}

void Screen::digAlign(byte dig, const char *string, PositionX position_x, PositionY position_y)
{
    if (dig > 9 && dig < 100)
    {
        digWidth = getStrWidth("W") * 2;
    }
    else
    {
        digWidth = getStrWidth("W");
    }

    if (string != 0)
    {
        digWidth += getStrWidth(string);
    }

    align(digWidth, getMaxCharWidth(), position_x, position_y);
    setCursor(x, y);

    print(dig);
    if (string != 0)
    {
        print(string);
    }
}

void Screen::textAlign(const char *string, PositionX position_x, PositionY position_y)
{
    align(getStrWidth(string), getMaxCharWidth(), position_x, position_y);
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
    if (!timer.escBar)
    {
        blockWidth = screenWidth / timer.maxEscapeCounter;
        timer.escBar = true;
    }

    width = blockWidth * (timer.maxEscapeCounter - timer.escapeCounter);
    drawBox(0, 58, width, 6);

    if (width == blockWidth * timer.maxEscapeCounter)
    {
        timer.escBar = false;
    }
}

void Screen::showBlink(Timer &timer)
{
    if (timer.blinkReady() && !timer.blinkHold)
    {
        frameAlign(digWidth + 10, getMaxCharWidth() + 10, centerX, centerFrame);
        digAlign(timer.counter, "", centerX, centerY);
    }
    else
    {
        digAlign(timer.counter, "", centerX, centerY);
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
        icon = 79;
    }
    if (buttonMinus.unlock)
    {
        icon = 68;
    }

    WH = 48;
    iconAlign(icon, WH, centerX, centerY);
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
    setFont(u8g2_font_HelvetiPixelOutline_tr);

    if (relayState.relaySW)
    {
        if (!buttonPlus.manualSwitch)
        {
            textAlign(vacState[1], left, up);

            setFont(u8g2_font_courB18_tr);
            digAlign(timer.counter, textCounter, right, down);
        }

        else
        {
            textAlign(vacState[1], centerX, centerY);
        }
    }

    else
    {
        textAlign(vacState[0], centerX, centerY);
    }
}

void Screen::vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.setTimerFlag && !buttonPlus.setTimerFlag && !timer.escBar)
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
                bottomLine(buttonMinus, buttonPlus, timer);
            }
        } while (nextPage());
    }
}