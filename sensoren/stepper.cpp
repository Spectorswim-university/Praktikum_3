#include "stepper.h"
#include <wiringPi.h>



Stepper::Stepper() {

}

void Stepper::initialize() {
    for (int i = 0; i < pins.size(); i++) {
        pinMode(pins[i], OUTPUT);
    }
    step = 0;
    direction = 1;
}


void Stepper::run() {
    // clockwise, then counter-clockwise
    // unipolar stepper motor (common ground, four coils in order: 1A, 1B, 2A, 2B)
    // halfstepping
    for (int p = 0; p < pins.size(); ++p) {
        digitalWrite(pins[p], (halfstepping[step % halfstepping.size()] & (1 << (pins.size() - p - 1))));
    }
    step += direction;
    int steps_per_rev = steps * reduction_gear * 2;
    delayMicroseconds(4 * 4000000ULL / steps_per_rev);

    if (step < 0 || step >= steps_per_rev)
    {
        direction = -direction;
        step += 2 * direction;
    }
}

void Stepper::shutdown() {
    for (int i = 0; i < pins.size(); i++) {
        digitalWrite(pins[i], 0);
    }
}
