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

void Buttons::readTimer()
{
    EEPROM.get(couterAddr, counter);

    // if (counter == 255)
    // {
    //     counter = defaultCounter;
    // }
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

void Buttons::resetSet()
{
    setTimerFlag = false;
    writeTimer();
}

boolean Buttons::changeTimer(boolean minus, boolean plus)
{
    blinkHide = false;

    if (minus)
    {
        counter--;
        blinkHide = true;
    }

    else if (plus)
    {
        counter++;
        blinkHide = true;
    }

    counter = constrain(counter, minSetCounter, maxSetCounter);

    return blinkHide;
}

void Buttons::setTimer()
{
    if (!setTimerFlag && !block)
    {
        if (blue.isClick() || blue.isHold() || red.isClick() || red.isHold())
        {
            if (manualSwitch)
            {
                manualSwitch = false;
            }

            if (pedalSwitch)
            {
                resetTimer();
            }

            setTimerFlag = true;
        }
    }

    if (setTimerFlag)
    {
        if (timer[0].ready(escapeCounter, changeTimer((blue.isClick() || blue.isHold()), (red.isClick() || red.isHold()))))
        {
            resetSet();

            if (pedalSwitch)
            {
                startTimer = true;
                readTimer();
            }
        }
    }
}

void Buttons::buttons()
{
    if (!pedal.isClick() || !pedal.isHold())
    {
        setTimer();

        if (blue.isHold() && red.isHold() && !manualSwitch)
        {
            if (setTimerFlag)
            {
                resetSet();
            }

            manualSwitch = true;

            pedalSwitch = false;
            block = true;
        }

        if (block && timer[1].wait(500))
        {
            block = false;
        }
    }
}

void Buttons::pedalCommands()
{
    if (pedal.isClick() || pedal.isHold())
    {
        pedalSwitch = true;

        if (setTimerFlag)
        {
            resetSet();
        }

        if (startTimer)
        {
            resetTimer();
        }

        if (manualSwitch)
        {
            manualSwitch = false;
        }
    }

    else if (pedal.isRelease() && !startTimer)
    {
        startTimer = true;
    }

    if (startTimer)
    {
        counter = timer[2].counter(counter);

        if (counter == 0)
        {
            pedalSwitch = false;
            resetTimer();
        }
    }
}

void Buttons::manualMode()
{
    if (manualSwitch)
    {
        manualCounter = timer[3].counter(manualCounter, false, reset);
        reset = false;

        if (manualCounter == 0)
        {
            manualSwitch = false;
            manualCounter = 10;
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