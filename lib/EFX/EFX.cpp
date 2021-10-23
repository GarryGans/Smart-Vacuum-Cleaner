#include "EFX.h"

// EFX::EFX() : U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/U8X8_PIN_NONE)
EFX::EFX() : U8G2_SH1106_128X64_NONAME_1_HW_I2C(U8G2_R0, /* reset=*/U8X8_PIN_NONE)
{
}

EFX::~EFX()
{
}

void EFX::alignSimbols(byte W, byte H, PosX pos_x, PosY pos_y)
{
    switch (pos_x)
    {
    case PosX::center:
        x = (screenWidth - W) / 2;
        break;

    case PosX::centerFrame:
        x = (screenWidth - W) / 2 + width + width / 2;
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

    case PosX::rightFrameSide:
        x = screenWidth - width - (screenWidth - W) / 2;
        break;

    case PosX::rightFrameHalfSide:
        x = screenWidth - width - (screenWidth / 2 - W) / 2;
        break;

    case PosX::custom:
        x = setX;
        break;

    case PosX::customFrame:
        x = setX - borderW / 2;
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

    case PosY::downFrameSpace:
        y = screenHeight - H - H / 4;
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

byte EFX::nextY(byte num, byte id)
{
    byte Y;

    Y = (screenHeight / num + height) / 2 + (screenHeight / num) * id;

    return Y;
}

template <typename type>
byte EFX::getDigWidth(type value)
{
    byte count;

    if (value == 0)
    {
        count = 1;
    }
    else
    {
        count = byte(log10(value) + 1);
    }

    char val[count];
    String(value).toCharArray(val, count);

    return getStrWidth(val);
}

void EFX::setHeight(const uint8_t *font)
{
    setFont(font);

    if (font == u8g2_font_courB08_tn || font == u8g2_font_courB08_tf)
    {
        height = 6;
    }

    else if (font == u8g2_font_pressstart2p_8f)
    {
        height = 8;
    }

    else if (font == u8g2_font_pixelmordred_tf || font == u8g2_font_HelvetiPixelOutline_tr)
    {
        height = 12;
    }

    else if (font == u8g2_font_profont22_tn || font == u8g2_font_9x18_tn || font == u8g2_font_crox4h_tf || font == u8g2_font_crox4hb_tf || font == u8g2_font_luBS14_te)
    {
        height = 14;
    }
    else if (font == u8g2_font_crox5tb_tf || font == u8g2_font_crox5tb_tf)
    {
        height = 16;
    }
    else if (font == u8g2_font_ncenB18_tf)
    {
        height = 18;
    }

    // else
    // {
    //     Serial.println("undefiner");
    // }
}

void EFX::textAlign(const char *string, PosX pos_x, PosY pos_y)
{
    alignSimbols(getStrWidth(string), height, pos_x, pos_y);

    setCursor(x, y);
    print(string);
}

void EFX::stringAlign(String str, PosX pos_x, PosY pos_y)
{
    char light[str.length() + 1];

    String(str).toCharArray(light, str.length() + 1);

    textAlign(light, pos_x, pos_y);
}

void EFX::digStringAlign(byte dig, const char *string, PosX pos_x, PosY pos_y)
{
    alignSimbols(getDigWidth(dig) + getStrWidth(string), height, pos_x, pos_y);

    setX = x;

    setCursor(x, y);

    print(dig);
    print(string);
}

void EFX::strDigAlign(const char *string, byte dig, PosX pos_x, PosY pos_y)
{
    String space;

    byte strW;

    if (getStrWidth(string) > getStrWidth("WW"))
    {
        space = " ";
        strW = getStrWidth(" ");
    }
    else
    {
        space = "  ";
        strW = getStrWidth("  ");
    }

    alignSimbols(getStrWidth(string) + strW + getDigWidth(dig), height, pos_x, pos_y);

    setX = x;

    setCursor(x, y);

    print(string);
    print(space);
    print(dig);
}

void EFX::strDigAlign(const String string, byte dig, PosX pos_x, PosY pos_y)
{
    char str[string.length() + 1];
    String(string).toCharArray(str, string.length() + 1);

    String space;

    byte strW;

    if (getStrWidth(str) > getStrWidth("WW"))
    {
        space = " ";
        strW = getStrWidth(" ");
    }
    else
    {
        space = "  ";
        strW = getStrWidth("  ");
    }

    alignSimbols(getStrWidth(str) + strW + getDigWidth(dig), height, pos_x, pos_y);

    setX = x;

    setCursor(x, y);

    print(str);
    print(space);
    print(dig);
}

template <typename type>
void EFX::digAlign(type dig, PosX pos_x, PosY pos_y)
{
    alignSimbols(getDigWidth(dig), height, pos_x, pos_y);

    setCursor(x, y);
    print(dig);
}

void EFX::digAlign(byte dig, PosX pos_x, PosY pos_y)
{
    alignSimbols(getDigWidth(dig), height, pos_x, pos_y);

    setCursor(x, y);
    print(dig);
}

void EFX::setPosition(const char *format, PosX pos_x, PosY pos_y)
{
    alignSimbols(getStrWidth(format), height, pos_x, pos_y);

    setCursor(x, y);
}

void EFX::iconAlign(int icon, byte iconWH, PosX pos_x, PosY pos_y)
{
    alignSimbols(iconWH, iconWH, pos_x, pos_y);
    drawGlyph(x, y, icon);
}

void EFX::frameAlign(byte W, byte H, PosX pos_x, PosY pos_y)
{
    borderW = 6;
    borderH = 6;

    W += borderW;
    H += borderH;

    alignSimbols(W, H, pos_x, pos_y);
    drawFrame(x, y, W, H);
}

void EFX::blinkFrame(int value, boolean dig, PosX pos_x, PosY pos_y, boolean tempBlock)
{
    if (!tempBlock)
    {
        if (timer.alternation(blinkMil))
        {
            if (dig)
            {
                width = getMaxCharWidth() * 2;
            }
            else
            {
                width = getDigWidth(value);
            }

            frameAlign(width, height, pos_x, pos_y);
        }
    }
}

void EFX::blinkFrame(const char *format, byte digAmount, PosX pos_x, PosY pos_y, boolean tempBlock)
{
    if (!tempBlock)
    {
        if (timer.alternation())
        {
            width = getMaxCharWidth() * digAmount;

            setPosition(format, pos_x, pos_y);

            borderW = 6;
            borderH = 6;

            width += borderW;
            height += borderH;

            drawFrame(x - borderW / 2, y - borderH / 2, width, height);
        }
    }
}

void EFX::mover(byte &move_x, byte deep_x)
{
    if (moveLeft)
    {
        move_x--;
        if (move_x == start_x - deep_x)
        {
            moveLeft = false;
            moveRight = true;
        }
    }
    else if (moveRight)
    {
        move_x++;
        if (move_x == deep_x + start_x)
        {
            moveRight = false;
            moveLeft = true;
        }
    }
}

void EFX::moveString(const char *string, PosX pos_x, PosY pos_y)
{

    setPosition(string, pos_x, pos_y);

    if (!move)
    {
        move = true;
        move_x = start_x = x;
        moveLeft = true;
        moveRight = false;
    }

    if (start_x != x)
    {
        start_x = x;

        if (move_x > 2 * start_x)
        {
            move_x = 2 * start_x;
            moveRight = false;
            moveLeft = true;
        }
    }

    setCursor(move_x, y);
    print(string);

    static Timer run;

    if (run.wait(50))
    {
        mover(move_x, start_x);
    }
}

void EFX::escapeBar()
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