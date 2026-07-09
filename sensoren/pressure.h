#pragma once

#include <cstdint>
#include <array>

class Pressure {
    public:
        Pressure();
        uint8_t device_id = 0x76;
        int dig_T[3] = {0};
        int dig_P[9] = {0};
        int fd;
        int8_t i2c_reg_write_self(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
        int8_t i2c_reg_read_self(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);

        void initialize();
        void run();
        void shutdown();
};
