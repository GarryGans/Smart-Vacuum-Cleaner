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
        setFont(u8g2_font_HelvetiPixelOutline_tr);
        setCursor(29, 18);
        print("SMART");

        setFont(u8g2_font_pixelmordred_tf);
        setCursor(0, 42);
        print("Vacuum Cleaner");

        setCursor(10, 59);
        setFont(u8g2_font_t0_12b_tf);
        print("iGor Studio 2020");
    } while (nextPage());
}

boolean Screen::blinkReady()
{
    
    if (millis() - prewCursorMillis >= blinkMillis)
    {
        blink = true;
        if (millis() - prewCursorMillis >= blinkMillis * 2)
        {
            prewCursorMillis = millis();
            blink = false;
        }
    }
    
    return blink;
}

void Screen::showBlink(int value)
{
    if (blinkReady())
    {
        if (value > 9)
        {
            print("__");
        }
        else
        {
            print("_");
        }        
    }
    else
    {
        print(value);
    }
}

void Screen::showTimerSet(Timer &timer)
{
    setFont(u8g2_font_crox2cb_tf);
    setCursor(20, 18);
    print("Set Timer");

    if (timer.counter > 9)
    {
        setCursor(48, 43);
    }
    else
    {
        setCursor(57, 43);
    }
    setFont(u8g2_font_courB18_tr);
    // print(timer.counter);
    showBlink(timer.counter);

    setCursor(5, 59);
    setFont(u8g2_font_t0_12b_tf);
    print("Hold minus to escape");
}

void Screen::setTimerScreen(Buttons &buttonMinus, Timer &timer)
{
    if (buttonMinus.setTimerFlag)
    {
        firstPage();
        do
        {
            showTimerSet(timer);
        } while (nextPage());
    }
}

void Screen::showAlert()
{
    print("manual mode");
}

void Screen::bottomLine(Buttons &buttonPlus)
{
    if (buttonPlus.manualSwitch)
    {
        setCursor(12, 59);
        setFont(u8g2_font_pixelmordred_tf);
        showAlert();
    }
    else
    {
        setCursor(17, 59);
        setFont(u8g2_font_pixelmordred_tf);
        print("auto mode ");
    }
}

void Screen::showVacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer)
{
    if (relayState.relaySW && !buttonPlus.manualSwitch)
    {
        setFont(u8g2_font_courB18_tr);
        if (timer.counter > 9)
        {
            setCursor(32, 20);
        }
        else
        {
            setCursor(47, 20);
        }

        print(timer.counter);
        print("s");
    }
    else
    {
        setFont(u8g2_font_Born2bSportySlab_tf);
        setCursor(7, 18);
        print("Are You Ready ?");
    }

    setFont(u8g2_font_9x15B_mr);
    setCursor(5, 40);
    print("VACUUM");

    setFont(u8g2_font_HelvetiPixelOutline_tr);
    setCursor(75, 40);
    if (relayState.relaySW)
    {
        print(vacState[1]);
    }
    else
    {
        print(vacState[0]);
    }
}

void Screen::vacuumScreen(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.setTimerFlag)
    {
        firstPage();
        do
        {
            // highLighter(keyboard);
            showVacuumState(relayState, buttonPlus, timer);
            bottomLine(buttonPlus);
        } while (nextPage());
    }
}