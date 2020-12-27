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
    buttonPlus.vacuumState = swOFF;
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
            timer.escapeMenuTimer();
            if (timer.escapeCounter == 0)
            {
                timer.writeTimer();
                setTimerFlag = false;
                buttonPlus.setTimerFlag = false;
            }
        }

        if (isClick())
        {
            timer.counterHold = true;
            if (!setTimerFlag)
            {
                totalOFF(motor, buttonPlus, timer);
                setTimerFlag = true;
                buttonPlus.setTimerFlag = true;
                
            }

            else if (setTimerFlag)
            {
                timer.updateEscape();
                minus = true;
                timer.changeTimer(minus, plus);
            }
        }

        if (isHold() && (setTimerFlag || buttonPlus.setTimerFlag))
        {
            timer.counterHold = true;
            timer.updateEscape();
            minus = true;
            timer.changeTimer(minus, plus);
        }

        if (isRelease())
        {
            timer.unFreezeBlink();
        }

        if (isHolded() && !(setTimerFlag || buttonPlus.setTimerFlag))
        {
            buttonLock = true;
            totalOFF(motor, buttonPlus, timer);
        }
    }

    else if (isHolded() && buttonLock)
    {
        buttonLock = false;
    }
}

void Buttons::redButton(Buttons &buttonMinus, Buttons &buttonPlus, Buttons &motor, Timer &timer)
{

    if (!buttonMinus.buttonLock)
    {
        tick();
        if (buttonMinus.setTimerFlag || setTimerFlag)
        {
            timer.escapeMenuTimer();
            if (timer.escapeCounter == 0)
            {
                timer.writeTimer();
                buttonMinus.setTimerFlag = false;
                setTimerFlag = false;
            }
        }

        if (isClick())
        {
            timer.counterHold = true;
            if (!buttonMinus.setTimerFlag || !setTimerFlag)
            {
                setTimerFlag = true;
                buttonMinus.setTimerFlag = true;

                totalOFF(motor, buttonPlus, timer);
            }

            else if (buttonMinus.setTimerFlag || setTimerFlag)
            {

                timer.updateEscape();
                plus = true;
                timer.changeTimer(minus, plus);
            }
        }
        if (isHolded())
        {
            if (!buttonMinus.setTimerFlag && !setTimerFlag)
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
                        motor.motorSwitch = false;
                        vacuumState = swOFF;
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
        if (isHold() && (buttonMinus.setTimerFlag || setTimerFlag))
        {
            timer.counterHold = true;
            timer.updateEscape();
            plus = true;
            timer.changeTimer(minus, plus);
        }
        if (isRelease())
        {
            timer.unFreezeBlink();
        }
    }
}

void Buttons::motorCommands(Buttons &buttonMinus, Buttons &buttonPlus, Timer &timer)
{
    if (!buttonMinus.buttonLock)
    {
        tick();
        if (isClick() || isHolded())
        {
            
            resetMotor = false;
            if (buttonMinus.setTimerFlag || buttonPlus.setTimerFlag)
            {
                timer.writeTimer();
                buttonMinus.setTimerFlag = false;
                buttonPlus.setTimerFlag = false;
            }
            timer.timerReset();
            motorSwitch = true;

            buttonPlus.manualSwitch = false;
            buttonPlus.vacuumState = swOFF;       
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
            }
        }
    }
}

void Buttons::commands(Buttons &buttonMinus, Buttons &buttonPlus, Buttons motor, Timer &timer)
{
    buttonMinus.blueButton(motor, buttonPlus, timer);
    buttonPlus.redButton(buttonMinus,buttonPlus, motor, timer);
    motor.motorCommands(buttonMinus, buttonPlus, timer);
}
