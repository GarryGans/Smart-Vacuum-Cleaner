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

void Buttons::motorState(boolean &motorSwitch, boolean state, Timer &timer, boolean &resetMotor)
{
    motorSwitch = state;
    resetMotor = false;
    timer.readTimer();
}

void Buttons::totalOFF(Buttons &motor, boolean &manualSwitch, Timer &timer)
{
    if (motor.motorSwitch)
    {
        motorState(motor.motorSwitch, false, timer, motor.resetMotor);
    }
    if (manualSwitch)
    {
        manualSwitch = false;
    }
}

void Buttons::blueButton(Buttons &motor, Buttons &buttonPlus, Timer &timer)
{
    tick();

    if (!lock)
    {
        if (!timer.setTimerFlag)
        {
            if (isClick())
            {
                totalOFF(motor, buttonPlus.manualSwitch, timer);
                timer.setTimerFlag = true;
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
            totalOFF(motor, buttonPlus.manualSwitch, timer);
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

void Buttons::redButton(Buttons &buttonMinus, Buttons &motor, Timer &timer)
{
    if (!buttonMinus.lock)
    {
        tick();

        if (!timer.setTimerFlag)
        {
            if (isClick())
            {
                totalOFF(motor, manualSwitch, timer);
                timer.setTimerFlag = true;
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
            if (!(motor.isClick() || motor.isHold()))
            {
                if (motor.motorSwitch || manualSwitch)
                {
                    totalOFF(motor, manualSwitch, timer);
                }
                else if (!manualSwitch)
                {
                    manualSwitch = true;
                }
            }
        }
    }
}

void Buttons::motorCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.lock)
    {
        tick();
        if (isClick() || isHold())
        {
            timer.resetEscape();

            motorState(motorSwitch, true, timer, resetMotor);

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
                motorState(motorSwitch, false, timer, resetMotor);
            }
        }
    }
}
