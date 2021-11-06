#define vacPin 3

#include <Arduino.h>
#include <Screen.h>
#include <Buttons.h>
#include <Switchers.h>
#include <Timer.h>


Buttons buttons;
Switchers switchers;
Screen screen;
Timer timer;

void setup()
{
  Serial.begin(9600);

  

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
  // buttonMinus.blueButton(buttonPlus, pedal, timer);
  // buttonPlus.redButton(buttonMinus, pedal, timer);
  // pedal.pedalCommands(buttonMinus, buttonPlus, timer);

  // switchers.switcher(buttonPlus, pedal);

  // screen.screens(switchers, pedal, buttonMinus, buttonPlus, timer);
}