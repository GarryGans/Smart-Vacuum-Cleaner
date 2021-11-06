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

void Buttons::escSet()
{
    if (!escBar && setTimerFlag)
    {
        // setTimerFlag = false;
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

void Buttons::setTimer(boolean &manualSwitch, Operator state)
{
    if (!setTimerFlag)
    {
        if (buttonMinus.isClick() || buttonMinus.isHold() || buttonPlus.isClick() || buttonPlus.isHold())
        {
            Serial.println("set");
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
        if (buttonMinus.isClick() || buttonMinus.isHold() || buttonPlus.isClick() || buttonPlus.isHold())
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

        if (buttonMinus.isRelease() || buttonPlus.isRelease())
        {
            blinkHide = false;
        }

        escSet();
    }
}

void Buttons::blueButton()
{
    // buttonMinus.tick();

    if (!pedal.isClick() || !pedal.isHold())
    {
        setTimer(manualSwitch, decrease);
    }
}

void Buttons::redButton()
{
    // buttonPlus.tick();

    if (!pedal.isClick() || !pedal.isHold())
    {
        setTimer(manualSwitch, increase);

        if (buttonMinus.isHold() && buttonPlus.isHold() && !manualSwitch)
        {
            // resetEscape();
            manualSwitch = true;
            pedalSwitch = false;
        }
    }
}

void Buttons::pedalCommands()
{
    // pedal.tick();

    if (pedal.isClick() || pedal.isHold())
    {
        pedalSwitch = true;

        // resetEscape();

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
    blueButton();
    redButton();
    pedalCommands();
}