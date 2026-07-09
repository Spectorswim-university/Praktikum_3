#include "sonic.h"
#include <chrono>
#include <wiringPi.h>

using Clock = std::chrono::high_resolution_clock;

Sonic::Sonic() {
}

void Sonic::initialize()
{
    pinMode(echo, INPUT);
    pinMode(trig, OUTPUT);
    pullUpDnControl(trig, PUD_UP);
}

float Sonic::run()
{
    // send ultrasonic impulse
    digitalWrite(trig, 1);
    delay(10);
    digitalWrite(trig, 0);

    // wait for echo
    while (digitalRead(echo) == 0);
    const Clock::time_point start = Clock::now();
    while (digitalRead(echo) == 1 );
    const Clock::time_point end = Clock::now();

    // calculate distance from time
    const std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start); // us
    const float distance = 1.0e-6f * duration.count() * 343.0f * 100.0f * 0.5f; // cm
    return distance; //in cm
}

void Sonic::shutdown()
{
    digitalWrite(echo, 0);
    pullUpDnControl(trig, PUD_OFF);
}

