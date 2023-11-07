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

    EEPROM.get(couterAddr, autoCounter);
}

boolean Buttons::blueB()
{
    return blue.isClick() || blue.isHold();
}

boolean Buttons::redB()
{
    return red.isClick() || red.isHold();
}

boolean Buttons::changeTimer(boolean minus, boolean plus)
{
    if (minus && autoCounter > minSetCounter)
    {
        autoCounter--;
        return true;
    }

    else if (plus && autoCounter < maxSetCounter)
    {
        autoCounter++;
        return true;
    }

    return false;
}

void Buttons::setTimer()
{
    if (!(pedal.isClick() || pedal.isHold()))
    {
        if (!setTimerFlag)
        {
            if (blueB() || redB())
            {
                startTimer = false;

                EEPROM.get(couterAddr, autoCounter);

                manualSwitch = false;

                setTimerFlag = true;

                reset_B = true;
            }
        }

        if (setTimerFlag)
        {
            blinkHide = changeTimer(blueB(), redB());

            byte temp = timer[0].reduceCounter(escCount, reset_B );

            reset_B = blinkHide;

            if (temp == 0)
            {
                EEPROM.put(couterAddr, autoCounter);
                setTimerFlag = false;

                EEPROM.get(couterAddr, autoCounter);

                if (pedalSwitch)
                {
                    reset_A = true;
                    correct = true;
                }
            }
        }
    }
}

void Buttons::manualMode()
{
    if (blue.isHold() && red.isHold() && !manualSwitch)
    {
        manualSwitch = true;

        pedalSwitch = false;
        startTimer = false;

        setTimerFlag = false;

        reset_M = true;

        block = true;
    }

    if (block && timer[1].wait(1000))
    {
        block = false;
    }

    if (manualSwitch)
    {
        manualCounter = timer[3].reduceCounter(manDef, reset_M);

        // reset_M = false;

        if (manualCounter == 0)
        {
            manualSwitch = false;
        }
    }
}

void Buttons::pedalMode()
{
    if (pedal.isClick() || pedal.isHold())
    {
        pedalSwitch = true;
        manualSwitch = false;

        startTimer = false;

        if (setTimerFlag)
        {
            EEPROM.put(couterAddr, autoCounter);
            setTimerFlag = false;
        }

        EEPROM.get(couterAddr, autoCounter);

        reset_A = true;
    }

    else if (pedalSwitch && pedal.isRelease() && !startTimer && !setTimerFlag)
    {
        startTimer = true;
    }

    if (startTimer && !setTimerFlag)
    {
        reset_A = false;

        if (escape)
        {
            pedalSwitch = false;
            startTimer = false;
            escape = false;
        }
    }

    if (correct)
    {
        startTimer = true;
        correct = false;
    }
}

void Buttons::com()
{
    blue.tick();
    red.tick();
    pedal.tick();

    setTimer();

    manualMode();
    pedalMode();
}