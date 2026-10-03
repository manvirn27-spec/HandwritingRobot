#include <Arduino.h>
#include "App.h"
#include "Buzzer.h"

App app;
Buzzer buz;

void setup() {
  app.begin("/gcode/path.gcode");
}
void loop(){
  app.execute();
}