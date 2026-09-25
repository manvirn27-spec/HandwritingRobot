#ifndef GCODEPARSER_H
#define GCODEPARSER_H

#include <Arduino.h>
#include <array>
#include <string>

class GcodeParser{
    public:
        enum class Command : int8_t{
            invalid = -1,
            liftPen = 0,
            lowerPen = 1,
            move = 3, 
            stop = 4,
            home = 5
        };

        Command parseLine(const String &line);
        Command getNextCommand();
        std::array<float, 3> getNextPos();
    private:
};

#endif