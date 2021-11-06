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

  buttons.begin();
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
  buttons.com();

  // switchers.switcher(buttonPlus, pedal);

  // screen.screens(switchers, pedal, buttonMinus, buttonPlus, timer);
}