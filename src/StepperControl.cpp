#include "StepperControl.h"


void StepperControl::begin(){
    pinMode(xLimitSwitch, INPUT_PULLUP);
    pinMode(yLimitSwitch, INPUT_PULLUP);
    pinMode(zLimitSwitch, INPUT_PULLUP);

    stepperX.setMaxSpeed(5000);
    stepperY.setMaxSpeed(5000);
    stepperZ.setMaxSpeed(5000);

    steppers.addStepper(stepperX);
    steppers.addStepper(stepperY);
    steppers.addStepper(stepperZ);

    pinMode(sleepPin, OUTPUT);
    digitalWrite(sleepPin, HIGH);
}

void StepperControl::home(){

    digitalWrite(xAxisDir, HIGH); 
    while (!digitalRead(xLimitSwitch)) { 
        digitalWrite(xAxisStep, HIGH);
        delayMicroseconds(300); 
        digitalWrite(xAxisStep, LOW); 
        delayMicroseconds(300);
    }
    stepperX.setCurrentPosition(0);
    delay(500);


    digitalWrite(yAxisDir, HIGH); 
    while (!digitalRead(yLimitSwitch)) { 
        digitalWrite(yAxisStep, HIGH);
        delayMicroseconds(300); 
        digitalWrite(yAxisStep, LOW); 
        delayMicroseconds(300);
    }
    stepperY.setCurrentPosition(0);
    delay(500);

    digitalWrite(zAxisDir, LOW); 
    while (!digitalRead(zLimitSwitch)) { 
        digitalWrite(zAxisStep, HIGH);
        delayMicroseconds(100); 
        digitalWrite(zAxisStep, LOW); 
        delayMicroseconds(100);
    }
    stepperZ.setCurrentPosition(0);
    delay(500);

    long positions[] = {20, 20, 20};
    steppers.moveTo(positions);
}

void StepperControl::move(float x, float y, float z){

    long positions[3];
    positions[0] = x * xStepstoMM; // X target step
    positions[1] = y * yStepstoMM;  // Y target step
    positions[2] = z * zStepstoMM;  // Z target step

    steppers.moveTo(positions);
    steppers.runSpeedToPosition(); // Blocks until complete
}
void StepperControl::stop(){
    stepperX.stop();
    stepperY.stop();
    stepperZ.stop();
}

void StepperControl::printAll(){
    Serial.printf("X Limit: %d, Y Limit: %d, Z Limit: %d \n",
        digitalRead(xLimitSwitch), digitalRead(yLimitSwitch), digitalRead(zLimitSwitch));
}