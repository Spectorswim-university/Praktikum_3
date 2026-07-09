#include <led.h>
#include <wiringPi.h> 
#include <iostream>


Led::Led() {
}

void Led::initialize() {
    std::cout << pins.size() << std::endl;
    for (int i = 0; i < pins.size(); i++) {
        pinMode(pins[i], OUTPUT);
    }
}

void Led::run() {
    for (int i = 0; i < pins.size(); i++) {
        digitalWrite(pins[i], HIGH);
        delay(330);
    }
    
    delay(2000);
    
    for (int i = 0; i < pins.size(); i++) {
        digitalWrite(pins[i], LOW);
        delay(330);
    }
}

void Led::shutdown() {
    for (int i = 0; i < pins.size(); i++) {
        digitalWrite(pins[i], LOW);
    }
}



