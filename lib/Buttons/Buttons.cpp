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

void Buttons::motorState(boolean &pedalSwitch, boolean state, Timer &timer, boolean &resetMotor)
{
    pedalSwitch = state;
    resetMotor = false;
    timer.readTimer();
}

void Buttons::totalOFF(Buttons &pedal, boolean &manualSwitch, Timer &timer)
{
    if (pedal.pedalSwitch)
    {
        motorState(pedal.pedalSwitch, false, timer, pedal.resetMotor);
    }
    if (manualSwitch)
    {
        manualSwitch = false;
    }
}

void Buttons::blueButton(Buttons &pedal, Buttons &buttonPlus, Timer &timer)
{
    if (!(pedal.isClick() || pedal.isHold()))
    {
        tick();

        if (!lock)
        {
            if (!timer.setTimerFlag)
            {
                if (isClick() && !buttonPlus.manualSwitch)
                {
                    timer.setTimerFlag = true;
                }
                else if (isClick() && buttonPlus.manualSwitch)
                {
                    totalOFF(pedal, buttonPlus.manualSwitch, timer);
                }
            }

            else if (timer.setTimerFlag)
            {
                if (isClick() || isHold())
                {
                    minus = true;
                    timer.changeTimer(minus, plus);
                }

                if (isRelease())
                {
                    timer.blinkHide = false;
                }

                timer.startEscape();
            }

            if (isHolded() && !timer.setTimerFlag)
            {
                lock = true;
                totalOFF(pedal, buttonPlus.manualSwitch, timer);
            }
        }

        else if (lock)
        {
            if (isHolded())
            {
                unlock = true;
            }
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

        if (!timer.setTimerFlag)
        {
            if (isClick() && !manualSwitch)
            {
                timer.setTimerFlag = true;
            }
            else if (isClick() && manualSwitch)
            {
                totalOFF(pedal, manualSwitch, timer);
            }
        }

        else if (timer.setTimerFlag)
        {
            if (isClick() || isHold())
            {
                plus = true;
                timer.changeTimer(minus, plus);
            }

            if (isRelease())
            {
                timer.blinkHide = false;
            }

            timer.startEscape();
        }

        if (isHolded() && !timer.setTimerFlag)
        {
            if (pedal.pedalSwitch || !manualSwitch)
            {
                manualSwitch = true;
                motorState(pedal.pedalSwitch, false, timer, resetMotor);
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

            motorState(pedalSwitch, true, timer, resetMotor);

            if (buttonPlus.manualSwitch)
            {
                buttonPlus.manualSwitch = false;
            }
        }

        if (isRelease() && !resetMotor)
        {
            resetMotor = true;
        }

        if (resetMotor)
        {
            if (timer.reduceTimer(timer.counter))
            {
                motorState(pedalSwitch, false, timer, resetMotor);
            }
        }
    }
}
