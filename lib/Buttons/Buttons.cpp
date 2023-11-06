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

    EEPROM.get(couterAddr, counter);
}

boolean Buttons::blueB()
{
    return blue.isClick() || blue.isHold();
}

boolean Buttons::redB()
{
    return red.isClick() || red.isHold();
}

void Buttons::writeTimer()
{
    EEPROM.put(couterAddr, counter);
    // reset_A = true;
    setTimerFlag = false;
}

void Buttons::manualSw()
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
}

boolean Buttons::changeTimer(boolean minus, boolean plus)
{
    if (minus && counter > minSetCounter)
    {
        counter--;
        return true;
    }

    else if (plus && counter < maxSetCounter)
    {
        counter++;
        return true;
    }

    return false;
}

void Buttons::setTimer()
{
    if (!setTimerFlag && !block)
    {
        if (blueB() || redB())
        {
            manualSwitch = false;

            setTimerFlag = true;

            reset_B = true;

            Serial.print("reset_B ");
            Serial.println(reset_B);
        }
    }

    if (setTimerFlag)
    {
        Serial.print("reset_B 2 ");
        Serial.println(reset_B);

        blinkHide = changeTimer(blueB(), redB());

        temp = timer[0].reduceCounter(escCount, reset_B);

        reset_B = blinkHide;

        if (temp == 0)
        {
            writeTimer();
            Serial.println("ZERO");
        }
    }
}

void Buttons::buttons()
{
    manualSw();

    if (!pedalSwitch)
    {
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

        reset_A = true;

        autoCounter = counter;

        if (setTimerFlag)
        {
            writeTimer();
            reset_B = true;
        }
    }

    else if (pedalSwitch && pedal.isRelease() && !startTimer && !setTimerFlag)
    {
        startTimer = true;
    }

    if (startTimer && !setTimerFlag)
    {
        autoCounter = timer[2].reduceCounter(counter, reset_A);

        reset_A = false;

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
        manualCounter = timer[3].reduceCounter(manDef, reset_M);

        reset_M = false;

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