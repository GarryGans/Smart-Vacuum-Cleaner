#define plusPin 5
#define minusPin 4
#define pedalPin 2
#define vacPin 3

#include <Arduino.h>
#include <Screen.h>
#include <Buttons.h>
#include <Switchers.h>
#include <Timer.h>


Buttons buttonPlus(plusPin);
Buttons buttonMinus(minusPin);
Buttons pedal(pedalPin);
Switchers switchers;
Screen screen;
Timer timer;

void setup()
{
  // Serial.begin(9600);

  buttonPlus.begin();
  buttonMinus.begin();
  pedal.begin();

  

  delay(100);

  switchers.begin(vacPin);
  delay(100);

  screen.begin();
  delay(100);
  screen.logo();
  delay(500);
}

void loop()
{
  buttonMinus.blueButton(buttonPlus, pedal, timer);
  buttonPlus.redButton(buttonMinus, pedal, timer);
  pedal.pedalCommands(buttonMinus, buttonPlus, timer);

  switchers.switcher(buttonPlus, pedal);

  screen.screens(switchers, buttonMinus, buttonPlus, timer);
}