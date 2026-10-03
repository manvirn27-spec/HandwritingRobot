#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <array>
#include "StepperControl.h"
#include "GCodeHandler.h"
#include "Buzzer.h"


class App{
    public:
        void begin(const char* filePath);
        void execute();
    private:
        StepperControl steppers;
        Buzzer buz;
        GCodeHandler parse;

        std::array<float, 3> currentPos;

        const float liftPenHeight = 30;
        const float lowerPenHeight = 0;        

};

#endif