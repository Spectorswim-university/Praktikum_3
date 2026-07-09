#include "movement.h"
#include <wiringPi.h>
#include <iostream>

Movement::Movement(){
}

void Movement::initialize()
{
    pinMode(pin, INPUT);

}

int Movement::run()
{
    return digitalRead(pin); 
    delay(1000);
}

void Movement::shutdown()
{
}
