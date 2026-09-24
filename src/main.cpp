#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "StepperControl.h"














/* Working example
AccelStepper stepperX(AccelStepper::DRIVER, 33, 12);
AccelStepper stepperY(AccelStepper::DRIVER, 32, 14);
AccelStepper stepperZ(AccelStepper::DRIVER, 25, 27);

MultiStepper steppers;

void setup() {
  // Configure max speeds
  stepperX.setMaxSpeed(2000);
  stepperY.setMaxSpeed(2000);
  stepperZ.setMaxSpeed(1000);

  // Add to multi-stepper group
  steppers.addStepper(stepperX);
  steppers.addStepper(stepperY);
  steppers.addStepper(stepperZ);

  stepperX.setCurrentPosition(0);
  stepperY.setCurrentPosition(0);
  stepperZ.setCurrentPosition(0);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
}

void loop() {
  long positions[3];
  positions[0] = 1000; // X target step
  positions[1] = 500;  // Y target step
  positions[2] = 200;  // Z target step

  // Moves all 3 motors so they arrive at their destination at the EXACT same time
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until complete
}
*/