#include <Arduino.h>
#include "App.h"
#include "Buzzer.h"

App app;
Buzzer buz;

void setup() {
  //app.begin("/gcode/path.gcode");
  buz.begin();
}
void loop(){
  //app.execute();
  buz.playStartup();
  delay(3000);
  buz.playError();
  delay(3000);
  buz.playEnd();
  delay(3000);
  buz.playShutdown();
  delay(5000);
}