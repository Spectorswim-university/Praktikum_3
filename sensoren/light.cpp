#include "light.h"
#include <stdexcept>
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "unistd.h"
#include <iostream>

Light::Light() {
}

void Light::initialize() {
    fd = wiringPiI2CSetup(device_id);
}

void Light::run() {
    uint8_t buffer[2] = {0};
    buffer[0] = 0x21; // One Time H-Resolution Mode2
    if (write(fd, buffer, 1) != 1)
        throw std::runtime_error("could not enter One Time H-Resolution Mode2!");
    if (read(fd, buffer, 2) != 2)
        throw std::runtime_error("reading measurement result failed!");

    const uint16_t intensity = (static_cast<uint16_t>(buffer[0]) << 8) | buffer[1];
    const float intensity_f = static_cast<float>(intensity) / 1.2f; // Lux
    std::cout << "I=" << std::to_string(intensity_f) << "lx" << std::endl;
}

void Light::shutdown() {
    wiringPiI2CWrite(fd, 0x00);
}
