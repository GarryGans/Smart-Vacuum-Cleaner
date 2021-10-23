#include "Screen.h"

Screen::Screen() : EFX()
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
        setHeight(u8g2_font_nokiafc22_tr);
        textAlign("Smart", PosX::center, PosY::upHalf);

        textAlign("Vacuum Cleaner", PosX::center, PosY::center);

        setHeight(u8g2_font_t0_12b_tf);
        textAlign("Garik 2020", PosX::center, PosY::downSpace);
    } while (nextPage());
}

void Screen::setTimerScreen(Timer &timer)
{
    setHeight(u8g2_font_courB18_tr);

    // digAlign(timer.counter, PosX::center, PosY::center);
    // blinkFrame(timer.counter, false, PosX::center, PosY::centerFrame, timer);

    escapeBar();
}

void Screen::bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (buttonPlus.manualSwitch)
    {
        setHeight(u8g2_font_pixelmordred_tf);

        moveString("manual mode", PosX::center, PosY::downSpace, 0);
    }
}

void Screen::vacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer)
{
    if (relayState.relaySW && !buttonPlus.manualSwitch)
    {
        pos_x = PosX::leftHalf;
        pos_y = PosY::upHalf;

        setHeight(u8g2_font_courB18_tr);
        digStringAlign(buttonPlus.counter, textCounter, PosX::rightHalf, PosY::downHalf);
    }

    else
    {
        pos_x = PosX::center;
        pos_y = PosY::center;
    }

    setHeight(u8g2_font_HelvetiPixelOutline_tr);

    textAlign(vacState[relayState.relaySW], pos_x, pos_y);
}

void Screen::screens(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    firstPage();
    do
    {
        if (buttonMinus.setTimerFlag || buttonPlus.setTimerFlag)
        {
            setTimerScreen(timer);
        }

        else
        {
            vacuumState(relayState, buttonPlus, timer);
            bottomLine(buttonMinus, buttonPlus, timer);
        }

    } while (nextPage());
}