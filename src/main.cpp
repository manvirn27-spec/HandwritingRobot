#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "GCodeHandler.h"
#include "StepperControl.h"
#include "GcodeParser.h"
#include "Buzzer.h"

StepperControl steppers;
GCodeHandler parser;

void setup() {
  Serial.begin(115200);
  parser.begin("/gcode/path.gcode");
  parser.dryRun();
}
void loop(){
}