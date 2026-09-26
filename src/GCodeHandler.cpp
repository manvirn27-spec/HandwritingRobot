#include "GCodeHandler.h"

bool GCodeHandler::begin(const char* filePath) {
    if (!LittleFS.begin(true)) {
        return false;
    }
    gcodeFile = LittleFS.open(filePath, "r");
    return gcodeFile == true;
}

GCodeHandler::Command GCodeHandler::getNextCommand() {
    if (!gcodeFile || !gcodeFile.available()) {
        return Command::endPage;
    }

    while (gcodeFile.available()) {
        String line = gcodeFile.readStringUntil('\n');
        line.trim();

        if (line.length() == 0 || line.startsWith(";")) {
            continue;
        }

        for (char c : line) {
            gcode.AddCharToLine(c);
        }
        gcode.AddCharToLine('\n'); 
        gcode.ParseLine(); 

        if (gcode.HasWord('G')) {
            int gVal = gcode.GetWordValue('G');
            if (gVal == 0 || gVal == 1) return Command::move;
            if (gVal == 2) return Command::liftPen;
            if (gVal == 3) return Command::lowerPen;
        } else if (gcode.HasWord('M')) {
            int mVal = gcode.GetWordValue('M');
            if (mVal == 0) return Command::home;
            if (mVal == 1) return Command::endPage;
        }
        return Command::invalid; 
    }

    return Command::endPage;
}

std::array<float, 3> GCodeHandler::getNextPos() {
    std::array<float, 3> pos = {0.0f, 0.0f, 0.0f};

    if (gcode.HasWord('X')) pos[0] = gcode.GetWordValue('X');
    if (gcode.HasWord('Y')) pos[1] = gcode.GetWordValue('Y');
    if (gcode.HasWord('Z')) pos[2] = gcode.GetWordValue('Z');

    return pos;
}
void GCodeHandler::closeFile() {
    if (gcodeFile) {
        gcodeFile.close();
    }
}
void GCodeHandler::dryRun() {
    Serial.println("==========================================");
    Serial.printf("STARTING DRY RUN \n"); 
    Serial.println("==========================================");

    int lineCount = 0;
    Command cmd;

    while ((cmd = getNextCommand()) != Command::endPage) {
        lineCount++;
        Serial.printf("[%03d] ", lineCount);

        switch (cmd) {
            case Command::move: {
                std::array<float, 3> pos = getNextPos();
                Serial.printf("ACTION: MOVE -> X: %.2f mm, Y: %.2f mm, Z: %.2f mm\n", pos[0], pos[1], pos[2]);
                break;
            }
            case Command::home:
                Serial.println("ACTION: HOME ALL AXES (G28)");
                break;

            case Command::lowerPen:
                Serial.println("ACTION: LOWER PEN (M3)");
                break;

            case Command::liftPen:
                Serial.println("ACTION: LIFT PEN (M5)");
                break;

            case Command::stop:
                Serial.println("ACTION: PROGRAM STOP / END (M0/M30)");
                break;

            case Command::invalid:
            default:
                Serial.println("ACTION: SKIPPED / UNKNOWN COMMAND");
                break;
        }
    }

    closeFile();
    Serial.println("==========================================");
    Serial.println("DRY RUN COMPLETE");
    Serial.println("==========================================");
}
void GCodeHandler::testFromInternalString() {
    Serial.println("==========================================");
    Serial.println("STARTING TEST FROM EMBEDDED G-CODE STRING");
    Serial.println("==========================================");

    std::stringstream stream(defaultGcode);
    std::string lineStr;
    int lineCount = 0;

    while (std::getline(stream, lineStr)) {
        String line = String(lineStr.c_str());
        line.trim();

        if (line.length() == 0 || line.startsWith(";")) {
            continue;
        }

        for (char c : line) {
            gcode.AddCharToLine(c);
        }
        gcode.AddCharToLine('\n');
        gcode.ParseLine();

        Command cmd = Command::invalid;

        if (gcode.HasWord('G')) {
            int gVal = gcode.GetWordValue('G');
            if (gVal == 0) cmd = Command::stop;
            if (gVal == 1) cmd = Command::move;
            if (gVal == 2) cmd = Command::liftPen;
            if (gVal == 3) cmd = Command::lowerPen;
        } else if (gcode.HasWord('M')) {
            int mVal = gcode.GetWordValue('M');
            if (mVal == 0) cmd = Command::home;
            if (mVal == 1) cmd = Command::endPage;
        }

        lineCount++;
        Serial.printf("[%03d] ", lineCount);

        switch (cmd) {
            case Command::move: {
                std::array<float, 3> pos = getNextPos();
                Serial.printf("ACTION: MOVE -> X: %.2f mm, Y: %.2f mm, Z: %.2f mm\n", pos[0], pos[1], pos[2]);
                break;
            }
            case Command::home:
                Serial.println("ACTION: HOME ALL AXES (M0)");
                break;
            case Command::lowerPen:
                Serial.println("ACTION: LOWER PEN (G3)");
                break;
            case Command::liftPen:
                Serial.println("ACTION: LIFT PEN (G2)");
                break;
            case Command::stop:
                Serial.println("ACTION: PROGRAM STOP (G0)");
                break;
            case Command::endPage:
                Serial.println("ACTION: END PAGE (M1)");
                break;
            case Command::invalid:
            default:
                Serial.println("ACTION: SKIPPED / UNKNOWN COMMAND");
                break;
        }

        if (cmd == Command::endPage) {
            break;
        }
    }

    Serial.println("==========================================");
    Serial.println("EMBEDDED STRING TEST COMPLETE");
    Serial.println("==========================================");
}