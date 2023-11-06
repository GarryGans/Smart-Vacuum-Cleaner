#define vacPin 3

#include <Arduino.h>
#include <Screen.h>
#include <Buttons.h>
#include <Switchers.h>

Buttons buttons;
Switchers switchers;
Screen screen;

void setup()
{
  Serial.begin(9600);

  buttons.begin();
  delay(100);

  switchers.begin(vacPin);
  delay(100);

  screen.begin();
  delay(100);

  // screen.logo();
  // delay(1000);
}

void loop()
{
  buttons.com();
  screen.screens(buttons);
  switchers.switcher(buttons);
}