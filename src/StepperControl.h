#ifndef STEPPERCONTROL_H
#define STEPPERCONTROL_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

class StepperControl{
    public:
        void begin();
        void home();
        void enable();
        void disable();

        void move(float x, float y, float z); //in mm. Blocking
        void stop();

        void printAll(); //for debugging
    private:
        const uint8_t xAxisStep = 33;
        const uint8_t xAxisDir = 12;
        const uint8_t yAxisStep = 32;
        const uint8_t yAxisDir = 14;
        const uint8_t zAxisStep = 25;
        const uint8_t zAxisDir = 27;

        AccelStepper stepperX{AccelStepper::DRIVER, xAxisStep, xAxisDir};
        AccelStepper stepperY{AccelStepper::DRIVER, yAxisStep, yAxisDir};
        AccelStepper stepperZ{AccelStepper::DRIVER, zAxisStep, zAxisDir};

        MultiStepper steppers;

        const int xLimitSwitch = 4;
        const int yLimitSwitch = 16;
        const int zLimitSwitch = 17;

        const float xStepstoMM = -36.f;
        const float yStepstoMM = -36.f; 
        const float zStepstoMM = 36.f;

        const float sleepPin = 15;
};

#endif