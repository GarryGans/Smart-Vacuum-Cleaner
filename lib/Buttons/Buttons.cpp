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
    setClickTimeout(300); // настройка таймаута между кликами (по умолчанию 300 мс)

    // HIGH_PULL - кнопка подключена к GND, пин подтянут к VCC (PIN --- КНОПКА --- GND)
    // LOW_PULL  - кнопка подключена к VCC, пин подтянут к GND
    setType(HIGH_PULL);

    // NORM_OPEN - нормально-разомкнутая кнопка
    // NORM_CLOSE - нормально-замкнутая кнопка
    setDirection(NORM_OPEN);
}

void Buttons::setTimer(Buttons &motor, Buttons &buttonPlus, Timer &timer)
{
    tick();
    if (isClick())
    {
        if (!setTimerFlag)
        {
            setTimerFlag = true;
            buttonPlus.manualSwitch = false;
            buttonPlus.vacuumState = swOFF;
            motor.motorSwitch = false;
            timer.timerReset();
        }

        else if (setTimerFlag)
        {
            minus = true;
            timer.changeTimer(minus, plus);
        }
    }
    if (isHolded())
    {
        timer.writeTimer();
        setTimerFlag = false;
    }
}

void Buttons::buttonCommands(Buttons &buttonMinus, Buttons &motor, Timer &timer)
{
    tick();
    if (isClick())
    {
        if (buttonMinus.setTimerFlag)
        {
            plus = true;
            timer.changeTimer(minus, plus);
        }
        else if (!buttonMinus.setTimerFlag)
        {
            manualSwitch = true;
            if (motor.motorSwitch)
            {
                motor.resetMotor = false;
                timer.timerReset();
            }

            switch (vacuumState)
            {
            case swOFF:

                if (!motor.motorSwitch)
                {
                    vacuumState = swON;
                }
                else if (motor.motorSwitch)
                {
                    vacuumState = swOFF;
                    motor.motorSwitch = false;
                }
                break;

            case swON:

                if (motor.motorSwitch)
                {
                    motor.motorSwitch = false;
                }
                vacuumState = swOFF;
                break;

            default:
                break;
            }
        }
    }
}

void Buttons::motorCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.setTimerFlag)
    {
        tick();
        if (isClick() || isHolded())
        {
            resetMotor = false;
            timer.timerReset();
            buttonPlus.manualSwitch = false;
            buttonPlus.vacuumState = swOFF;
            motorSwitch = true;
        }

        if (isRelease() && !buttonPlus.manualSwitch && !resetMotor)
        {
            resetMotor = true;
        }

        else if (resetMotor)
        {
            timer.minusCounter();
            if (timer.counter == 0)
            {
                motorSwitch = false;
                resetMotor = false;
                timer.timerReset();
            }
        }
    }
}