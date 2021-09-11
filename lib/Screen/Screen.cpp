#include "Screen.h"

Screen::Screen() : U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/U8X8_PIN_NONE)
{
}

Screen::~Screen()
{
}

void Screen::align(byte W, byte H, PosX pos_x, PosY pos_y)
{
    switch (pos_x)
    {
    case PosX::center:
        x = (screenWidth - W) / 2;
        break;

    case PosX::left:
        x = 0;
        break;

    case PosX::leftSpace:
        x = W / 8;
        break;

    case PosX::leftHalf:
        x = (screenWidth / 2 - W) / 2;
        break;

    case PosX::right:
        x = screenWidth - W;
        break;

    case PosX::rightSpace:
        x = screenWidth - (W + W / 8);
        break;

    case PosX::rightHalf:
        x = (screenWidth + screenWidth / 2 - W) / 2;
        break;

    case PosX::custom:
        x = setX;
        break;

    default:
        break;
    }

    switch (pos_y)
    {
    case PosY::center:
        y = (screenHeight + H) / 2;
        break;

    case PosY::up:
        y = H;
        break;

    case PosY::upSpace:
        y = H + H / 4;
        break;

    case PosY::upHalf:
        y = (screenHeight / 2 - H) / 2 + H;
        break;

    case PosY::down:
        y = screenHeight;
        break;

    case PosY::downSpace:
        y = screenHeight - H / 4;
        break;

    case PosY::downHalf:
        y = (screenHeight + screenHeight / 2 - H) / 2 + H;
        break;

    case PosY::centerFrame:
        y = (screenHeight - H) / 2;
        break;

    case PosY::upFrame:
        y = 0;
        break;

    case PosY::upFrameHalf:
        y = (screenHeight / 2 - H) / 2;
        break;

    case PosY::downFrame:
        y = screenHeight - H;
        break;

    case PosY::downFrameHalf:
        y = (screenHeight + screenHeight / 2 - H) / 2;
        break;

    case PosY::custom:
        y = setY;
        break;

    default:
        break;
    }
}

void Screen::mover(byte deep_x)
{
    if (move_x > (start_x - deep_x) && moveLeft)
    {
        move_x--;
        if (move_x == start_x - deep_x)
        {
            moveLeft = false;
            moveRight = true;
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

void Screen::moveString(Timer &timer, byte deep_x, byte bottom_y, const char *str)
{
    if (!move)
    {
        move_x = (screenWidth - getStrWidth(str)) / 2;
        move = true;
        moveLeft = true;
        moveRight = false;
    }

    setCursor(move_x, bottom_y);
    print(str);

    if (timer.moveReady())
    {
        start_x = (screenWidth - getStrWidth(str)) / 2;
        deep_x = constrain(deep_x, 0, start_x);
        mover(deep_x);
    }
}

byte Screen::getDigWidth(byte value)
{
    char val[4];
    String(value).toCharArray(val, 4);

    return getStrWidth(val);
}

void Screen::digStringAlign(byte dig, const char *str, PosX pos_x, PosY pos_y)
{
    align(getDigWidth(dig) + getStrWidth(str), getMaxCharWidth(), pos_x, pos_y);

    setCursor(x, y);

    print(dig);
    print(str);
}

void Screen::digAlign(byte dig, PosX pos_x, PosY pos_y)
{
    align(getDigWidth(dig), getMaxCharWidth(), pos_x, pos_y);

    setCursor(x, y);
    print(dig);
}

void Screen::frameAlign(byte W, byte H, PosX pos_x, PosY pos_y)
{
    W += W / 4;
    H += H / 2;

    align(W, H, pos_x, pos_y);
    drawFrame(x, y, W, H);
}

void Screen::textAlign(const char *str, PosX pos_x, PosY pos_y)
{
    align(getStrWidth(str), getMaxCharWidth(), pos_x, pos_y);

    setCursor(x, y);
    print(str);
}

void Screen::iconAlign(int icon, byte iconWH, PosX pos_x, PosY pos_y)
{
    align(iconWH, iconWH, pos_x, pos_y);
    drawGlyph(x, y, icon);
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

void Screen::blinkFrame(Timer &timer)
{
    if (timer.blinkReady())
    {
        frameAlign(getDigWidth(timer.counter), getMaxCharWidth(), PosX::center, PosY::centerFrame);
    }
}

void Screen::logo()
{
    firstPage();
    do
    {
        setFont(u8g2_font_nokiafc22_tr);
        textAlign("Smart", PosX::center, PosY::upHalf);

        textAlign("Vacuum Cleaner", PosX::center, PosY::center);

        setFont(u8g2_font_t0_12b_tf);
        textAlign("Garik 2020", PosX::center, PosY::downSpace);
    } while (nextPage());
}

void Screen::setTimerScreen(Timer &timer)
{
    setFont(u8g2_font_courB18_tr);

    blinkFrame(timer);
    digAlign(timer.counter, PosX::center, PosY::center);

    escapeBar(timer);
}

void Screen::bottomLine(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    setFont(u8g2_font_pixelmordred_tf);

    if (buttonPlus.manualSwitch)
    {
        moveString(timer, deep_x, bottom_y, "manual mode");
    }

    else
    {
        move = false;
    }
}

void Screen::vacuumState(Switchers &relayState, Buttons &buttonPlus, Timer &timer)
{
    setFont(u8g2_font_HelvetiPixelOutline_tr);

    if (relayState.relaySW && !buttonPlus.manualSwitch)
    {
        pos_x = PosX::leftHalf;
        pos_y = PosY::upHalf;

        setFont(u8g2_font_courB18_tr);
        digStringAlign(timer.counter, textCounter, PosX::rightHalf, PosY::downHalf);
    }

    else
    {
        pos_x = PosX::center;
        pos_y = PosY::center;
    }

    textAlign(vacState[relayState.relaySW], pos_x, pos_y);
}

void Screen::screens(Switchers &relayState, Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    firstPage();
    do
    {
        if (timer.setTimerFlag)
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