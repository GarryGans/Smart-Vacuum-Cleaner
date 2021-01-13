#include "Buttons.h"

Buttons::Buttons(int8_t pin) : GButton(pin)
{
}

Buttons::Buttons()
{
}

Buttons::~Buttons()
{
}

void Buttons::begin()
{
    setDebounce(50);      // настройка антидребезга (по умолчанию 80 мс)
    setTimeout(1000);     // настройка таймаута на удержание (по умолчанию 500 мс)
    setClickTimeout(200); // настройка таймаута между кликами (по умолчанию 300 мс)

    // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (PIN --- КНОПКА --- GND)
    // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
    setType(HIGH_PULL);

    // NORM_OPEN - нормально-разомкнутая кнопка
    // NORM_CLOSE - нормально-замкнутая кнопка
    setDirection(NORM_OPEN);
}

void Buttons::motorState(boolean &pedalSwitch, boolean state, Timer &timer, boolean &startTimer)
{
    pedalSwitch = state;
    startTimer = false;
    timer.readTimer();
}

void Buttons::totalOFF(Buttons &pedal, boolean &manualSwitch, Timer &timer)
{
    if (pedal.pedalSwitch)
    {
        motorState(pedal.pedalSwitch, false, timer, pedal.startTimer);
    }
    if (manualSwitch)
    {
        manualSwitch = false;
    }
}

void Buttons::setTimer(boolean manualSwitch, Timer &timer, Buttons &pedal, MP state)
{
    if (!manualSwitch)
    {
        if (!timer.setTimerFlag)
        {
            if (isClick())
            {
                timer.setTimerFlag = true;

                if (pedal.pedalSwitch)
                {
                    motorState(pedal.pedalSwitch, true, timer, pedal.startTimer);
                }
            }
        }

        else if (timer.setTimerFlag)
        {
            if (isClick() || isHold())
            {
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
                pedal.startTimer = true;
            }
        }
    }
}

void Buttons::blueButton(Buttons &pedal, Buttons &buttonPlus, Timer &timer)
{
    if (!(pedal.isClick() || pedal.isHold()))
    {
        tick();

        if (!lock)
        {
            setTimer(buttonPlus.manualSwitch, timer, pedal, decrease);

            if (isHolded() && !timer.setTimerFlag)
            {
                lock = true;
                totalOFF(pedal, buttonPlus.manualSwitch, timer);
            }
        }

        else if (lock && isHolded())
        {
            unlock = true;
        }

        if (unlock)
        {
            if (timer.reduceTimer(timer.unblockCounter))
            {
                lock = false;
                unlock = false;
                timer.maxUnblock();
            }
        }
    }
}

void Buttons::redButton(Buttons &buttonMinus, Buttons &pedal, Timer &timer)
{
    if (!buttonMinus.lock && !(pedal.isClick() || pedal.isHold()))
    {
        tick();

        setTimer(manualSwitch, timer, pedal, increase);

        if (isHolded() && !timer.setTimerFlag)
        {
            if (pedal.pedalSwitch)
            {
                motorState(pedal.pedalSwitch, false, timer, pedal.startTimer);
            }

            else if (!manualSwitch)
            {
                manualSwitch = true;
            }

            else if (manualSwitch)
            {
                manualSwitch = false;
            }
        }
    }
}

void Buttons::pedalCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.lock)
    {
        tick();
        if (isClick() || isHold())
        {
            timer.resetEscape();

            motorState(pedalSwitch, true, timer, startTimer);

            if (buttonPlus.manualSwitch)
            {
                buttonPlus.manualSwitch = false;
            }
        }

        if (isRelease() && !startTimer)
        {
            startTimer = true;
        }

        if (startTimer)
        {
            if (timer.reduceTimer(timer.counter))
            {
                motorState(pedalSwitch, false, timer, startTimer);
            }
        }
    }
}
