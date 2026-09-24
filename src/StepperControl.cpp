#include "StepperControl.h"

StepperControl::begin(){
    pinMode(xLimitSwitch, INPUT_PULLUP);
    pinMode(yLimitSwitch, INPUT_PULLUP);
    pinMode(zLimitSwitch, INPUT_PULLUP);

    stepperX.setMaxSpeed(2000);
    stepperY.setMaxSpeed(2000);
    stepperZ.setMaxSpeed(2000);

    steppers.addStepper(stepperX);
    steppers.addStepper(stepperY);
    steppers.addStepper(stepperZ);

    stepperX.setCurrentPosition(0);
    stepperY.setCurrentPosition(0);
    stepperZ.setCurrentPosition(0);

    pinMode(sleepPin, OUTPUT);
    digitalWrite(sleepPin, HIGH);
}

StepperControl::home(){
    stepperX.setSpeed(-500);
    while(digitalRead(xLimitSwitch))
        stepperX.runSpeed();
    stepperX.stop();
    stepperX.setCurrentPosition(0);

    delay(500);

    stepperY.setSpeed(-500);
    while(digitalRead(yLimitSwitch))
        stepperY.runSpeed();
    stepperY.stop();
    stepperY.setCurrentPosition(0);

    delay(500);

    stepperZ.setSpeed(-500);
    while(digitalRead(zLimitSwitch))
        stepperZ.runSpeed();
    stepperZ.stop();
    stepperZ.setCurrentPosition(0);
}

StepperControl::move(float x, float y, float z){
    
    long positions[3];
    positions[0] = x * ; // X target step
    positions[1] = y;  // Y target step
    positions[2] = z;  // Z target step

    steppers.moveTo(positions);
    steppers.runSpeedToPosition(); // Blocks until complete
}