#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "StepperControl.h"
#include "GcodeParser.h"
#include "Buzzer.h"

StepperControl steppers;
GcodeParser parser;

void setup() {
  Serial.begin(115200);
  steppers.begin();
  steppers.home();
}

void loop() {
  steppers.move(100, 200, 1000);
  delay(100);
  steppers.move(500, 50, 500);
  delay(100);
  steppers.move(1, 1, 1);
  delay(100);
}