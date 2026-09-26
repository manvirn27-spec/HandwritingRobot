#include "App.h"
#include <array>

void App::begin(){
    steppers.begin();
    steppers.home();
    currentPos[0] = 0; currentPos[1] = 0; currentPos[2] = 0;
}

void App::execute(){
    GCodeHandler::Command cmd = parse.getNextCommand();

    if(cmd == GCodeHandler::Command::move){
        std::array<float, 3> pos = parse.getNextPos();
        steppers.move(pos[0], pos[1], pos[2]);

        currentPos[0] = pos[0];
        currentPos[1] = pos[1];
        currentPos[2] = pos[2];
    }
    else if(cmd == GCodeHandler::Command::stop){
        steppers.stop();
    }
    else if(cmd == GCodeHandler::Command::liftPen){
        steppers.move(currentPos[0], currentPos[1], liftPenHeight);
    }
    else if(cmd == GCodeHandler::Command::lowerPen){
        steppers.move(currentPos[0], currentPos[1], lowerPenHeight);
    }
    else if(cmd == GCodeHandler::Command::home){
        steppers.home();
    }
    else{
        //beep error and block in while loop
        while(1){}
    }

}