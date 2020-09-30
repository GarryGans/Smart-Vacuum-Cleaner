#define plusPin 4
#define minusPin 5
#define motorPin 3
#define vacPin 2

#include <Arduino.h>
#include <Screen.h>
#include <Buttons.h>
#include <Switchers.h>
#include <Timer.h>

Buttons buttonPlus(plusPin);
Buttons buttonMinus(minusPin);
Buttons motor(motorPin);
Switchers switchers;
Screen screen;
Timer timer;

void setup()
{
  // Serial.begin(9600);

  timer.readTimer();
  timer.writeTimer();

  buttonPlus.begin();
  buttonMinus.begin();
  motor.begin();
  delay(100);

  switchers.begin(vacPin);
  delay(100);

  screen.begin();
  delay(100);
  screen.logo();
  delay(3000);
}

void loop()
{
  buttonMinus.setTimer(motor, buttonPlus, timer);
  buttonPlus.buttonCommands(buttonMinus, motor, timer);
  motor.motorCommands(buttonMinus, buttonPlus, timer);

  switchers.switcher(buttonPlus, motor);

  screen.vacuumScreen(switchers, buttonMinus, buttonPlus, timer);
  screen.setTimerScreen(buttonMinus, timer);
}