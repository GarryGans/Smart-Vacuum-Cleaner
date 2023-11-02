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
        setHeight(u8g2_font_courB08_tf);
        textAlign("Smart", PosX::center, PosY::upHalf);

        textAlign("Vacuum Cleaner", PosX::center, PosY::center);

        setHeight(u8g2_font_t0_12b_tf);
        textAlign("Garik 2020", PosX::center, PosY::downSpace);
    } while (nextPage());
}

void Screen::setTimerScreen(Buttons &buttons)
{
    setHeight(u8g2_font_profont22_tn);

    digAlign(buttons.counter, PosX::center, PosY::center);
    blinkFrame(buttons.counter, PosX::center, PosY::centerFrame, buttons.blinkHide, true);
}

void Screen::bottomLine(Buttons &buttons)
{
    if (buttons.manualSwitch)
    {
        setHeight(u8g2_font_pixelmordred_tf);

        moveString("manual mode", PosX::center, PosY::downSpace);
    }
}

void Screen::vacuumState(Buttons &buttons)
{
    if (buttons.pedalSwitch)
    {
        pos_x = PosX::leftHalf;
        pos_y = PosY::upHalf;

        setHeight(u8g2_font_profont22_tn);
        digStringAlign(buttons.counter, textCounter, PosX::rightHalf, PosY::downHalf);
    }

    if (buttons.manualSwitch)
    {
        pos_x = PosX::leftHalf;
        pos_y = PosY::center;

        setHeight(u8g2_font_profont22_tn);
        digStringAlign(buttons.manualCounter, textCounter, PosX::rightHalf, PosY::center);
    }

    if (!(buttons.pedalSwitch || buttons.manualSwitch))
    {
        pos_x = PosX::center;
        pos_y = PosY::center;
    }

    setHeight(u8g2_font_HelvetiPixelOutline_tr);

    moveString(vacState[(buttons.pedalSwitch || buttons.manualSwitch) ? true : false], pos_x, pos_y, 0, 0, buttons.manualSwitch ? 20 : 80);
}

void Screen::screens(Buttons &buttons)
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
            vacuumState(buttons);
            bottomLine(buttons);
        }

    } while (nextPage());
}