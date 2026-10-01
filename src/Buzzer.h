#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer{
    public:
        void begin();

        void playStartup();
        void playShutdown();
        void playError();
        void playEnd();

    private:
        const int buzzerPin = 1;
};

#endif