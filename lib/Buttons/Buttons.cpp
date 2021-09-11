#include "Buttons.h"

Buttons::Buttons(int8_t pin) : GButton(pin)
{
}

Buttons::~Buttons()
{
}

void Buttons::begin()
{
    setDebounce(50);      // настройка антидребезга (по умолчанию 80 мс)
    setTimeout(300);      // настройка таймаута на удержание (по умолчанию 500 мс)
    setClickTimeout(200); // настройка таймаута между кликами (по умолчанию 300 мс)

    // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (PIN --- КНОПКА --- GND)
    // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
    setType(HIGH_PULL);

    // NORM_OPEN - нормально-разомкнутая кнопка
    // NORM_CLOSE - нормально-замкнутая кнопка
    setDirection(NORM_OPEN);
}

void Buttons::setTimer(boolean &manualSwitch, Timer &timer, Buttons &pedal, Operator state)
{
    if (!timer.setTimerFlag)
    {
        if (isClick() || isHold())
        {
            if (manualSwitch)
            {
                manualSwitch = false;
            }

            timer.setTimerFlag = true;

            if (pedal.pedalSwitch)
            {
                timer.resetTimer();
            }
        }
    }

    if (timer.setTimerFlag)
    {
        if (isClick() || isHold())
        {
            if (manualSwitch)
            {
                manualSwitch = false;
            }

            switch (state)
            {
            case decrease:
                minus = true;
                break;

            case increase:
                plus = true;
                break;

            default:
                break;
            }

            timer.changeTimer(minus, plus);
        }

        if (isRelease())
        {
            timer.blinkHide = false;
        }

        timer.startEscSet();

        if (pedal.pedalSwitch)
        {
            timer.startTimer = true;
        }
    }
}

void Buttons::blueButton(Buttons &buttonPlus, Buttons &pedal, Timer &timer)
{
    if (!pedal.isClick() || !pedal.isHold())
    {
        tick();

        setTimer(buttonPlus.manualSwitch, timer, pedal, decrease);
    }
}

void Buttons::redButton(Buttons &buttonMinus, Buttons &pedal, Timer &timer)
{
    if (!pedal.isClick() || !pedal.isHold())
    {
        tick();

        setTimer(manualSwitch, timer, pedal, increase);

        if (buttonMinus.isHold() && isHold() && !manualSwitch)
        {
            timer.resetEscape();
            manualSwitch = true;
            pedal.pedalSwitch = false;
        }
    }
}

void Buttons::pedalCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    tick();

    if (isClick() || isHold())
    {
        pedalSwitch = true;

        timer.resetEscape();

        timer.resetTimer();

        if (buttonPlus.manualSwitch)
        {
            buttonPlus.manualSwitch = false;
        }
    }

    if (isRelease() && !timer.startTimer)
    {
        timer.startTimer = true;
    }

    if (timer.startTimer)
    {
        if (timer.reduceTimer(timer.counter))
        {
            pedalSwitch = false;
            timer.resetTimer();
        }
    }
}
