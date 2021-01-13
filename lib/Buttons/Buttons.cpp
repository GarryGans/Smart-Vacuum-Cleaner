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

void Buttons::choicePedalManual(boolean &pedalSwitch, boolean &startTimer, boolean &manualSwitch, Timer &timer, Choice choice)
{
    switch (choice)
    {
    case treadle:
        motorState(pedalSwitch, true, timer, startTimer);
        manualSwitch = false;
        break;

    case off:
        motorState(pedalSwitch, false, timer, startTimer);
        manualSwitch = false;
        break;

    default:
        break;
    }
}

void Buttons::setTimer(boolean manualSwitch, Timer &timer, Buttons &pedal, Operator state)
{
    if (!timer.setTimerFlag)
    {
        if (isClick())
        {
            if (!manualSwitch)
            {
                timer.setTimerFlag = true;

                if (pedal.pedalSwitch)
                {
                    pedal.startTimer = false;
                    timer.readTimer();
                }
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
                choicePedalManual(pedal.pedalSwitch, pedal.startTimer, buttonPlus.manualSwitch, timer, off);
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
            if (pedal.pedalSwitch || manualSwitch)
            {
                choicePedalManual(pedal.pedalSwitch, pedal.startTimer, manualSwitch, timer, off);
            }

            else if (!manualSwitch)
            {
                manualSwitch = true;
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
            choicePedalManual(pedalSwitch, startTimer, buttonPlus.manualSwitch, timer, treadle);
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
