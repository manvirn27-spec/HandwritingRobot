#ifndef GCODEHANDLER_H
#define GCODEHANDLER_H

#include <Arduino.h>
#include <array>
#include <sstream>
#include <FS.h>
#include <LittleFS.h>
#include <GCodeParser.h> 

class GCodeHandler {
    public:
        enum class Command : int8_t {
            invalid = -1,
            liftPen = 0,
            lowerPen = 1,
            move = 3, 
            stop = 4,
            home = 5,
            endPage = 6
        };

        bool begin(const char* filePath);
        Command getNextCommand();
        std::array<float, 3> getNextPos();

        void closeFile();

        void dryRun(); //testing
        void testFromInternalString(); //testing

    private:
        File gcodeFile;
        GCodeParser gcode;

        const char* defaultGcode = R"(
G0 ;stop
G1 X190.00 Y100.00 ;move to start
G3 ;lower pen
G1 X189.82 Y105.65 ;move
G1 X189.29 Y111.28 ;move
G1 X188.41 Y116.88 ;move
G1 X187.18 Y122.43 ;mov
G1 X224.42 Y80.82 ;move
G1 X222.40 Y87.36 ;move
G1 X219.92 Y93.85 ;move
G1 X216.97 Y100.25 ;move
G1 X213.58 Y106.53 ;move
G1 X209.75 Y112.67 ;move
G1 X205.50 Y118.64 ;move
G1 X200.83 Y124.41 ;move
G1 X195.78 Y129.96 ;move
G1 X190.35 Y135.26 ;move
G1 X190.00 Y100.00 ;return to initial vertex
G2 ;lift pen
G1 X10.00 Y10.00 ;return home
M1 ;end page note


)";
};

#endif