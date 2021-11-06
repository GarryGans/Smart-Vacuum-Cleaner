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

void Buttons::escSet()
{
    if (!escBar && setTimerFlag)
    {
        setTimerFlag = false;
        writeTimer();
    }
}

void Buttons::changeTimer(boolean minus, boolean plus)
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

    counter = constrain(counter, minSetCounter, maxSetCounter);
}

void Buttons::setTimer(boolean &manualSwitch, Timer &timer, Buttons &pedal, Operator state)
{
    if (!setTimerFlag)
    {
        if (isClick() || isHold())
        {
            if (manualSwitch && timer.wait(secMillis))
            {
                manualSwitch = false;
            }

            else if (pedal.pedalSwitch)
            {
                resetTimer();
            }

            setTimerFlag = true;
        }
    }

    if (setTimerFlag)
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

            changeTimer(minus, plus);
        }

        if (isRelease())
        {
            blinkHide = false;
        }

        escSet();
    }
}

void Buttons::blueButton(Buttons &buttonPlus, Buttons &pedal, Timer &timer)
{
    tick();

    if (!pedal.isClick() || !pedal.isHold())
    {
        setTimer(buttonPlus.manualSwitch, timer, pedal, decrease);
    }
}

void Buttons::redButton(Buttons &buttonMinus, Buttons &pedal, Timer &timer)
{
    tick();

    if (!pedal.isClick() || !pedal.isHold())
    {
        Serial.println("red");
        setTimer(manualSwitch, timer, pedal, increase);

        if (buttonMinus.isHold() && isHold() && !manualSwitch)
        {
            // resetEscape();
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

        // resetEscape();

        resetTimer();

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
        if (timer.ready(counter))
        {
            pedalSwitch = false;
            resetTimer();
        }
    }
}
