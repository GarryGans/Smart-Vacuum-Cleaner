#include "Screen.h"

Screen::Screen() : EFX()
{
}

Screen::~Screen()
{
}

void Screen::logo()
{
    // MACH MEMORY FLASH

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

    digAlign(buttons.autoCounter, PosX::center, PosY::center);
    blinkFrame(buttons.autoCounter, PosX::center, PosY::centerFrame, buttons.blinkHide, true);
}

void Screen::bottomLine(Buttons &buttons)
{
    if (buttons.manualSwitch)
    {
        setHeight(u8g2_font_pixelmordred_tf);

        moveStringPad(mode, PosX::center, PosY::upSpace, manStrPad, manStrTime);
    }
}

void Screen::vacuumState(Buttons &buttons)
{
    if (buttons.pedalSwitch)
    {
        pos_x = PosX::leftHalf;
        pos_y = PosY::upHalf;

        setHeight(u8g2_font_profont22_tn);
        digAlign(buttons.autoCounter, PosX::rightHalf, PosY::center);

        autoBar(buttons.autoCounter, buttons.escape, false, buttons.reset_A);
    }

    if (buttons.manualSwitch)
    {
        setHeight(u8g2_font_pixelmordred_tf);

        moveStringPad(mode, PosX::center, PosY::upSpace, manStrPad, manStrTime);

        pos_x = PosX::leftHalf;
        pos_y = PosY::center;

        setHeight(u8g2_font_profont22_tn);
        digAlign(buttons.manualCounter, PosX::rightHalf, PosY::center);
    }

    if (!(buttons.pedalSwitch || buttons.manualSwitch))
    {
        pos_x = PosX::center;
        pos_y = PosY::center;
    }

    setHeight(u8g2_font_HelvetiPixelOutline_tr);

    String state = vacState[(buttons.pedalSwitch || buttons.manualSwitch) ? true : false];

    byte time = buttons.manualSwitch ? manTime : autoTime;

    moveStringDeep(state, pos_x, pos_y, stateStrDeep, time);
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
            // bottomLine(buttons);
        }

    } while (nextPage());
}