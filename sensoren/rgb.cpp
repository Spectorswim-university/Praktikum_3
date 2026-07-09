#include "rgb.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "wiringPi.h"
#include "softPwm.h"
#include <cstdint>
#include <iostream>


// Konstruktor 

Rgb::Rgb() {

}

void Rgb::initialize() {
    wiringPiSetup();
    for (int i = 0; i < pins.size(); i++) {
        pinMode(pins[i], OUTPUT);
        softPwmCreate(pins[i], 0, period);
    }
    hue = 0.0f;
}

void Rgb::run() {
// sweep hue, max saturation, max value
    if (hue >= 360.0f)
        hue = 0.0f;

    const int h_i = hue / 60;
    const float t = hue / 60.0f - h_i;
    const float q = (1.0f - t);

    float r, g, b;
    switch (h_i)
    {
    default:
    case 0:
        r = 1.0f;
        g = t;
        b = 0.0f;
        break;
    case 1:
        r = q;
        g = 1.0f;
        b = 0.0f;
        break;
    case 2:
        r = 0.0f;
        g = 1.0f;
        b = t;
        break;
    case 3:
        r = 0.0f;
        g = q;
        b = 1.0f;
        break;
    case 4:
        r = t;
        g = 0.0f;
        b = 1.0f;
        break;
    case 5:
        r = 1.0f;
        g = 0.0f;
        b = q;
        break;
    }

    r *= 1.0;
    g *= 0.5;
    b *= 0.8;

    r = logf(1.0f + r) / logf(2.0f);
    g = logf(1.0f + g) / logf(2.0f);
    b = logf(1.0f + b) / logf(2.0f);

    r = std::clamp(r, 0.0f, 1.0f);
    g = std::clamp(g, 0.0f, 1.0f);
    b = std::clamp(b, 0.0f, 1.0f);

    softPwmWrite(pins[0], static_cast<int>(period * r));
    softPwmWrite(pins[1], static_cast<int>(period * g));
    softPwmWrite(pins[2], static_cast<int>(period * b));
    const float T = 0.1f * period;
    delay(static_cast<uint32_t>(T));

    hue += T / 25.0f;
}


void Rgb::shutdown() {
    for (int i = 0; i < pins.size(); i++) {
        softPwmWrite(pins[i], 0);
        digitalWrite(pins[i], 0);
    }
}



