#include "Buttons.h"

GButton buttonPlus(plusPin);
GButton buttonMinus(minusPin);
GButton pedal(pedalPin);

Buttons::Buttons()
{
}

Buttons::~Buttons()
{
}

void Buttons::set(GButton &butt)
{
    butt.setDebounce(50);      // настройка антидребезга (по умолчанию 80 мс)
    butt.setTimeout(300);      // настройка таймаута на удержание (по умолчанию 500 мс)
    butt.setClickTimeout(200); // настройка таймаута между кликами (по умолчанию 300 мс)

    // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (PIN --- КНОПКА --- GND)
    // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
    butt.setType(HIGH_PULL);

    // NORM_OPEN - нормально-разомкнутая кнопка
    // NORM_CLOSE - нормально-замкнутая кнопка
    butt.setDirection(NORM_OPEN);
}

void Buttons::begin()
{
    set(buttonPlus);
    set(buttonMinus);
    set(pedal);

    readTimer();
}

void Buttons::readTimer()
{
    EEPROM.get(couterAddr, counter);

    if (counter == 255)
    {
        counter = defaultCounter;
    }
}

void Buttons::writeTimer()
{
    EEPROM.put(couterAddr, counter);
}

void Buttons::resetTimer()
{
    startTimer = false;
    readTimer();
}

void Buttons::resetEscape()
{
    setTimerFlag = false;
    writeTimer();
}

boolean Buttons::changeTimer(boolean minus, boolean plus)
{
    blinkHide = true;

    if (minus)
    {
        counter--;
    }

    else if (plus)
    {
        counter++;
    }

    else
    {
        blinkHide = false;
    }

    counter = constrain(counter, minSetCounter, maxSetCounter);

    return blinkHide;
}

void Buttons::setTimer()
{
    if (!setTimerFlag)
    {
        if (buttonMinus.isClick() || buttonMinus.isHold() || buttonPlus.isClick() || buttonPlus.isHold())
        {
            if (manualSwitch && timer.wait(secMillis))
            {
                manualSwitch = false;
            }

            else if (pedalSwitch)
            {
                resetTimer();
            }

            setTimerFlag = true;
        }
    }

    if (setTimerFlag)
    {
        if (timer.ready(3, changeTimer((buttonMinus.isClick() || buttonMinus.isHold()), (buttonPlus.isClick() || buttonPlus.isHold()))))
        {
            resetEscape();
        }
    }
}

void Buttons::buttons()
{
    if (!pedal.isClick() || !pedal.isHold())
    {
        setTimer();

        if (buttonMinus.isHold() && buttonPlus.isHold() && !manualSwitch)
        {
            resetEscape();
            manualSwitch = true;
            pedalSwitch = false;
        }
    }
}

void Buttons::pedalCommands()
{
    if (pedal.isClick() || pedal.isHold())
    {
        pedalSwitch = true;

        resetEscape();

        resetTimer();

        if (manualSwitch)
        {
            manualSwitch = false;
        }
    }

    if (pedal.isRelease() && !startTimer)
    {
        startTimer = true;
    }

    if (startTimer)
    {
        if (timer.ready(counter))
        {
            pedalSwitch = false;
            resetTimer();
        }
    }
}

void Buttons::com()
{
    buttonMinus.tick();
    buttonPlus.tick();
    pedal.tick();

    buttons();
    pedalCommands();
}