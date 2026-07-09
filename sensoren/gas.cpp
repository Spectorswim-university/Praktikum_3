#include "gas.h"
#include <stdexcept>
#include "unistd.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include <iostream>

Gas::Gas() {
}

void Gas::initialize() {
    fd = wiringPiI2CSetup(device_id);
    if (fd == -1)
        throw std::runtime_error("wiringPiI2CSetup failed with code " + std::to_string(errno) + "!");

    // HW_ID
    uint8_t buffer[5] = {0};
    buffer[0] = 0x20;
    if (write(fd, buffer, 1) != 1)
        throw std::runtime_error("could not read HW_ID!");
    if (read(fd, buffer, 1) != 1)
        throw std::runtime_error("reading HW_ID failed!");
    if (buffer[0] != 0x81)
        throw std::runtime_error("hardware ID is wrong (expected 0x81, got " + std::to_string(buffer[0]) + ")!");

    // SW_RESET
    buffer[0] = 0xFF;
    buffer[1] = 0x11;
    buffer[2] = 0xE5;
    buffer[3] = 0x72;
    buffer[4] = 0x8A;
    if (write(fd, buffer, 5) != 5)
        throw std::runtime_error("could not reset device!");
    delay(2);

    // APP_VERIFY
    buffer[0] = 0xF3;
    if (write(fd, buffer, 1) != 1)
        throw std::runtime_error("could not verify app!");
    delay(70);

    // APP_START
    buffer[0] = 0xF4;
    if (write(fd, buffer, 1) != 1)
        throw std::runtime_error("could not start app!");
    delay(1);

    // STATUS
    buffer[0] = 0x00;
    if (write(fd, buffer, 1) != 1)
        throw std::runtime_error("could not read STATUS!");
    if (read(fd, buffer, 1) != 1)
        throw std::runtime_error("reading STATUS failed!");
    if (buffer[0] & 0x01)
        throw std::runtime_error("device reports an error!");
    if ((buffer[0] & 0xF0) != 0x90)
        throw std::runtime_error("invalid device state!");

    // MEAS_MODE
    // Mode 1 - Constant power mode
    // no interrupts
    buffer[0] = 0x01;
    buffer[1] = 0x10;
    if (write(fd, buffer, 2) != 2)
        throw std::runtime_error("could not write MEAS_MODE!");
}

void Gas::run() {
    // ALG_RESULT_DATA
    // 6 bytes: eCO2 high, eCO2 low, eTVOC high, eTVOC low, status, error
    // eCO2 from 400ppm to 32768ppm
    // eTVOC from 0ppb to 32768ppb
    uint8_t buffer[6] = {0};
    buffer[0] = 0x02;
    if (write(fd, buffer, 1) != 1)
        throw std::runtime_error("could not read ALG_RESULT_DATA!");
    if (read(fd, buffer, 6) != 6)
        throw std::runtime_error("reading ALG_RESULT_DATA failed!");
    if (buffer[4] & 0x08)
    {
        const uint16_t eco2 = (static_cast<uint16_t>(buffer[0]) << 8) | buffer[1];
        const uint16_t etvoc = (static_cast<uint16_t>(buffer[2]) << 8) | buffer[3];
        std::cout << "eCO2=" << std::to_string(eco2) <<  "ppm eTVOC=" << std::to_string(etvoc) << "ppb" << std::endl;
    }
}

void Gas::shutdown() {
    wiringPiI2CWriteReg8(fd, 0x01, 0x00);
}
