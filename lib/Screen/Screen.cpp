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
        setFont(u8g2_font_pixelmordred_tf);
        textAlign("smart", 18, center);
        textAlign("vacuum cleaner", 38, center);

        setFont(u8g2_font_t0_12b_tf);
        textAlign("2020", 59, center);
    } while (nextPage());
}

void Screen::showBlink(Timer &timer)
{
    if (timer.blinkReady() && !timer.counterHold)
    {
        if (timer.counter > 9)
        {
            textAlign("__", 43, center);
        }
        else
        {
            textAlign("_", 43, center);
        }
    }
    else
    {
        print(timer.counter);
    }
}

void Screen::escapeBar(Timer &timer)
{
    if (!widthGet)
    {
        blockWidth = screenWidth / timer.escapeCounter;
        widthGet = true;
    }

    drawBox(0, 50, blockWidth * timer.escapeCounter, 10);

    if (timer.escapeCounter == 0)
    {
        widthGet = false;
    }
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

void Screen::textAlign(const char *string, byte y, Position position)
{
    switch (position)
    {
    case left:
        x = (screenWidth / 2 - getStrWidth(string)) / 2;
        break;

    case right:
        x = (screenWidth + (screenWidth / 2) - getStrWidth(string)) / 2;
        break;

    case center:
        x = (screenWidth - getStrWidth(string)) / 2;
        break;

    
    default:
        break;
    }
    
    setCursor(x, y);
    print(string);
}

void Screen::showTimerSet(Timer &timer)
{
    setFont(u8g2_font_crox2cb_tf);
    textAlign("Set Timer", 18, center);

    if (timer.counter > 9)
    {
        setCursor(48, 43);
    }
    else
    {
        setCursor(57, 43);
    }
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

void Screen::blockScreen()
{
    setFont(u8g2_font_HelvetiPixelOutline_tr);
    textAlign("BLOCK !!!", 25, center);
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

    else if (buttonMinus.buttonLock)
    {
        moveString(timer, deep_x, bottom_y, "block mode ");
    }

    else
    {
        moveString(timer, deep_x, bottom_y, "auto mode ");
    }
}

void Screen::showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer)
{
    if (relayState.relaySW && !buttonPlus.manualSwitch)
    {
        setFont(u8g2_font_HelvetiPixelOutline_tr);
        if (relayState.relaySW)
        {
            textAlign(vacState[1], 30, left);
        }
        else
        {
            textAlign(vacState[0], 30, left);
        }

        setFont(u8g2_font_courB18_tr);
        // if (timer.counter > 9)
        // {
        //     setCursor(60, 33);
        // }
        // else
        // {
        //     setCursor(75, 33);
        // }
        char time[] = (char)timer.counter;
        textAlign(time, 33, right);
    }

    else
    {
        setFont(u8g2_font_HelvetiPixelOutline_tr);
        setCursor(48, 30);
        if (relayState.relaySW)
        {
            print(vacState[1]);
        }
        else
        {
            print(vacState[0]);
        }
    }
}

void Screen::vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.setTimerFlag && !buttonPlus.setTimerFlag)
    {
        firstPage();
        do
        {
            if (buttonMinus.buttonLock)
            {
                blockScreen();
            }
            else
            {
                showVacuumState(relayState, buttonPlus, timer);
            }

            bottomLine(buttonMinus, buttonPlus, timer);
        } while (nextPage());
    }
}