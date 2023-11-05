#include "Buttons.h"

GButton red(plusPin);
GButton blue(minusPin);
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
    set(red);
    set(blue);
    set(pedal);

    readTimer();
}

boolean Buttons::blueB()
{
    return blue.isClick() || blue.isHold();
}

boolean Buttons::redB()
{
    return red.isClick() || red.isHold();
}

void Buttons::readTimer()
{
    EEPROM.get(couterAddr, counter);
}

void Buttons::writeTimer()
{
    EEPROM.put(couterAddr, counter);
}

void Buttons::resetSet()
{
    setTimerFlag = false;
    writeTimer();
}

void Buttons::manualSw()
{
    if (blue.isHold() && red.isHold() && !manualSwitch)
    {
        manualSwitch = true;
        pedalSwitch = false;

        setTimerFlag = false;

        reset = true;

        block = true;
    }

    if (block && timer[1].wait(1000))
    {
        block = false;
    }
}

boolean Buttons::changeTimer(boolean minus, boolean plus)
{
    if (minus && counter > minSetCounter)
    {
        counter--;
        blinkHide = true;
    }

    else if (plus && counter < maxSetCounter)
    {
        counter++;
        blinkHide = true;
    }

    else
    {
        blinkHide = false;
    }

    return blinkHide;
}

void Buttons::setTimer()
{
    if (!setTimerFlag && !block)
    {
        if (blueB() || redB())
        {
            if (manualSwitch)
            {
                manualSwitch = false;
            }

            if (pedalSwitch)
            {
                startTimer = false;
            }

            reset = true;

            setTimerFlag = true;
        }
    }

    if (setTimerFlag)
    {
        temp = timer[0].reduceCounter(escCount, changeTimer(blueB(), redB()));

        if (temp == 0)
        {
            resetSet();
        }
    }
}

void Buttons::buttons()
{
    if (!pedal.isClick() || !pedal.isHold())
    {
        manualSw();
        setTimer();
    }
}

void Buttons::pedalCommands()
{
    if (pedal.isClick() || pedal.isHold())
    {
        pedalSwitch = true;
        manualSwitch = false;

        startTimer = false;

        reset = true;

        if (setTimerFlag)
        {
            resetSet();
        }
    }

    else if (pedalSwitch && pedal.isRelease() && !startTimer)
    {
        startTimer = true;
    }

    if (startTimer)
    {
        autoCounter = timer[2].reduceCounter(counter, reset);

        reset = false;

        if (autoCounter == 0)
        {
            pedalSwitch = false;
            startTimer = false;
        }
    }
}

void Buttons::manualMode()
{
    if (manualSwitch)
    {
        manualCounter = timer[3].reduceCounter(manDef, reset);

        reset = false;

        if (manualCounter == 0)
        {
            manualSwitch = false;
        }
    }
}

void Buttons::com()
{
    blue.tick();
    red.tick();
    pedal.tick();

    buttons();
    manualMode();
    pedalCommands();
}