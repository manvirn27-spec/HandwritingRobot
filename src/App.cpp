#include "App.h"
#include "GCodeHandler.h"

void App::begin(const char* filePath){
    steppers.begin();
    steppers.home();
    parse.begin("filePath"); //change to whatever filepath for the one you want to print
    buz.begin();
    buz.playStartup();

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
    else if(cmd == GCodeHandler::Command::endPage){
        Serial.println("Page ended. Replace sheet and select next file.");
        steppers.move(0, 0, 100);
        steppers.disable();
        buz.playEnd();
    }
    else{
        if(cmd == GCodeHandler::Command::invalid)
            Serial.println("Gcode error: Invalid command");
        else
            Serial.println("Unkown error");

        for(int i = 0; i < 10; i++){
            buz.playError();
            delay(5000);
        }
        while(1){}
    }

}