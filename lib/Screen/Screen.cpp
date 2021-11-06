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

void Screen::setTimerScreen(Buttons &buttons)
{
    setHeight(u8g2_font_courB18_tr);

    digAlign(buttons.counter, PosX::center, PosY::center);
    blinkFrame(buttons.counter, PosX::center, PosY::centerFrame, buttons.blinkHide, true);

    escapeBar(buttons.blinkHide);
}

void Screen::bottomLine(Buttons &buttons)
{
    if (buttons.manualSwitch)
    {
        setHeight(u8g2_font_pixelmordred_tf);

        moveString("manual mode", PosX::center, PosY::downSpace);
    }
}

void Screen::vacuumState(Switchers &relayState, Buttons &buttons)
{
    if (relayState.relaySW && !buttons.manualSwitch)
    {
        pos_x = PosX::leftHalf;
        pos_y = PosY::upHalf;

        setHeight(u8g2_font_courB18_tr);
        digStringAlign(buttons.counter, textCounter, PosX::rightHalf, PosY::downHalf);
    }

    else
    {
        pos_x = PosX::center;
        pos_y = PosY::center;
    }

    setHeight(u8g2_font_HelvetiPixelOutline_tr);

    textAlign(vacState[relayState.relaySW], pos_x, pos_y);
}

void Screen::screens(Switchers &relayState, Buttons &buttons)
{
    firstPage();
    do
    {
        if (buttons.setTimerFlag)
        {
            setTimerScreen(buttons);
        }

        else
        {
            vacuumState(relayState, buttons);
            bottomLine(buttons);
        }

    } while (nextPage());
}