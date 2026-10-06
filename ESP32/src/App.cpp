#include "App.h"
#include "GCodeHandler.h"

void App::begin(const char* filePath){
    steppers.begin();
    steppers.home();
    buz.begin();
    if(!parse.begin(filePath)){
        while(1){
            buz.playError();
            delay(5000);
        }   
    }
    buz.playStartup();

    currentPos[0] = 0; currentPos[1] = 0; currentPos[2] = 0;
}

void App::execute(){
    GCodeHandler::Command cmd = parse.getNextCommand();

    if(cmd == GCodeHandler::Command::move){
        std::array<float, 2> pos = parse.getNextPos();
        
        //if(pos[0] > xMaxPos) pos[0] = xMaxPos;
        //if(pos[0] < xMinPos) pos[0] = xMaxPos;
        //if(pos[0] > yMaxPos) pos[0] = yMaxPos;
        //if(pos[0] < yMinPos) pos[0] = yMaxPos;

        steppers.move(pos[0], pos[1], currentPos[2]);

        currentPos[0] = pos[0];
        currentPos[1] = pos[1];
    }
    else if(cmd == GCodeHandler::Command::stop){
        steppers.stop();
    }
    else if(cmd == GCodeHandler::Command::liftPen){
        steppers.move(currentPos[0], currentPos[1], liftPenHeight);
        currentPos[2] = liftPenHeight;
    }
    else if(cmd == GCodeHandler::Command::lowerPen){
        steppers.move(currentPos[0], currentPos[1], lowerPenHeight);
        currentPos[2] = lowerPenHeight;
    }
    else if(cmd == GCodeHandler::Command::home){
        steppers.home();
    }
    else if(cmd == GCodeHandler::Command::endPage){
        Serial.println("Page ended. Replace sheet and select next file.");
        steppers.move(0, 0, 100);
        steppers.disable();
        buz.playEnd();
        while(1);
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