#include <Arduino.h>
#include "App.h"

App app;


void setup() {
  app.begin("/gcode/path.gcode")
}
void loop(){
  app.execute();
}