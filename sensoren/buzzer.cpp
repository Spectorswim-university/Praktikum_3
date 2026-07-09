#include "buzzer.h"
#include <stdexcept>
#include "wiringPi.h"
#include "softTone.h"



Buzzer::Buzzer() {
}

void Buzzer::initialize() {
    pinMode(pin, OUTPUT);
    softToneCreate(pin);
}

void Buzzer::run() {
    for (int i = 0; i < melody.size(); i++) {
        softToneWrite(pin, melody[i]);
        delay(200);
    }
    softToneWrite(pin, 0);
    delay(1000);
}

void Buzzer::shutdown() {
    softToneWrite(pin, 0);
    digitalWrite(pin, 0);
}
