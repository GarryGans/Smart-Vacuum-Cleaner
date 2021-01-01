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

void Buttons::totalOFF(Buttons &motor, Buttons &buttonPlus, Timer &timer)
{
    buttonPlus.manualSwitch = false;
    if (motor.motorSwitch)
    {
        motor.motorSwitch = false;
        motor.resetMotor = false;
        timer.timerReset();
    }
}

void Buttons::blueButton(Buttons &motor, Buttons &buttonPlus, Timer &timer)
{
    tick();

    if (!buttonLock)
    {
        if (setTimerFlag || buttonPlus.setTimerFlag)
        {
            timer.startEscape(setTimerFlag, buttonPlus.setTimerFlag);
        }

        if (isClick())
        {
            timer.resetEscapeCount();

            if (!setTimerFlag)
            {
                totalOFF(motor, buttonPlus, timer);
                setTimerFlag = true;
                buttonPlus.setTimerFlag = true;
            }

            else if (setTimerFlag)
            {
                minus = true;
                timer.changeTimer(minus, plus);
            }
        }

        if (isHold() && (setTimerFlag || buttonPlus.setTimerFlag))
        {
            timer.resetEscapeCount();

            minus = true;
            timer.changeTimer(minus, plus);
        }

        if (isRelease())
        {
            timer.blinkHold = false;
        }

        if (isHolded() && !(setTimerFlag || buttonPlus.setTimerFlag))
        {
            buttonLock = true;
            totalOFF(motor, buttonPlus, timer);
        }
    }

    else if (isHolded() && buttonLock)
    {
        unlock = true;
    }
    if (unlock)
    {
        timer.escapeTimer(timer.unblockCounter);
        if (timer.unblockCounter == 0)
        {
            buttonLock = false;
            unlock = false;
            timer.unblockCounter = timer.maxUnblockCounter;
        }
    }
}

void Buttons::redButton(Buttons &buttonMinus, Buttons &buttonPlus, Buttons &motor, Timer &timer)
{

    if (!buttonMinus.buttonLock)
    {
        tick();
        if (buttonMinus.setTimerFlag || setTimerFlag)
        {
            timer.startEscape(buttonMinus.setTimerFlag, setTimerFlag);
        }

        if (isClick())
        {
            timer.resetEscapeCount();

            if (!buttonMinus.setTimerFlag || !setTimerFlag)
            {
                setTimerFlag = true;
                buttonMinus.setTimerFlag = true;

                totalOFF(motor, buttonPlus, timer);
            }

            else if (buttonMinus.setTimerFlag || setTimerFlag)
            {
                plus = true;
                timer.changeTimer(minus, plus);
            }
        }
        if (isHolded())
        {
            if (!buttonMinus.setTimerFlag && !setTimerFlag)
            {
                if (motor.motorSwitch)
                {
                    motor.motorSwitch = false;
                    motor.resetMotor = false;
                    timer.timerReset();
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
        if (isHold() && (buttonMinus.setTimerFlag || setTimerFlag))
        {
            timer.resetEscapeCount();

            plus = true;
            timer.changeTimer(minus, plus);
        }
        if (isRelease())
        {
            timer.blinkHold = false;
        }
    }
}

void Buttons::motorCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.buttonLock)
    {
        tick();
        if (isClick() || isHold())
        {
            timer.escBar = false;
            resetMotor = false;
            if (buttonMinus.setTimerFlag || buttonPlus.setTimerFlag)
            {
                buttonMinus.setTimerFlag = false;
                buttonPlus.setTimerFlag = false;
                timer.writeTimer();
            }
            else
            {
                timer.timerReset();
            }

            if (buttonPlus.manualSwitch)
            {
                buttonPlus.manualSwitch = false;
            }

            motorSwitch = true;
        }

        if (isRelease() && !buttonPlus.manualSwitch && !resetMotor)
        {
            resetMotor = true;
        }

        if (resetMotor)
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
