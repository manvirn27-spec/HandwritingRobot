#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

class StepperControl{
    public:
        begin();
        home();
        move(float x, float y, float z); //in mm. Blocking

        printAll(); //for debugging
    private:
        AccelStepper stepperX(AccelStepper::DRIVER, 33, 12);
        AccelStepper stepperY(AccelStepper::DRIVER, 32, 14);
        AccelStepper stepperZ(AccelStepper::DRIVER, 25, 27);

        MultiStepper steppers;

        const int xLimitSwitch = 4;
        const int yLimitSwitch = 16;
        const int zLimitSwitch = 17;

        const float xStepstoMM = 
        const float yStepstoMM = 
        const float zStepstoMM = 

        const float sleepPin = 15;
};

#endif