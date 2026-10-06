#include "Buzzer.h"

void Buzzer::begin(){
    pinMode(BUZZER_PIN, OUTPUT);
}
void Buzzer::playEnd(){
    tone(BUZZER_PIN, 523, 70); // C5
    delay(300);
    tone(BUZZER_PIN, 659, 70); // E5
    delay(300);
    tone(BUZZER_PIN, 784, 120); // G5 (slightly longer final note)
    delay(350);
    noTone(BUZZER_PIN);}
void Buzzer::playShutdown(){
    tone(BUZZER_PIN, 523, 120); // C5
    delay(340);
    tone(BUZZER_PIN, 392, 250); // G4
    delay(670);
    noTone(BUZZER_PIN);}
void Buzzer::playError(){
    for (int i = 0; i < 3; i++) {
        tone(BUZZER_PIN, 262, 60); // C4
        delay(190);
    }
    noTone(BUZZER_PIN);}
void Buzzer::playStartup(){
    tone(BUZZER_PIN, 392, 100); // G4
    delay(120);
    tone(BUZZER_PIN, 523, 200); // C5
    delay(220);
    noTone(BUZZER_PIN);
}