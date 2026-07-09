#include "pressure.h"
#include <vector>
#include <cstring>
#include <stdexcept>
#include "unistd.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"


Pressure::Pressure() {
}

void Pressure::initialize() {
    if (fd < 0)
    {
        fd = wiringPiI2CSetup(device_id);
        if (fd == -1)
            throw std::runtime_error("wiringPiI2CSetup failed with code " + std::to_string(errno) + "!");
    }

    uint8_t data[24] = {0};
    if (i2c_reg_read_self(device_id, 0x88, data, sizeof(data)))
        throw std::runtime_error("could not read coefficients!");
    dig_T[0] = data[1] * 256 + data[0];
	dig_T[1] = data[3] * 256 + data[2];
	if(dig_T[1] > 32767)
		dig_T[1] -= 65536;
	dig_T[2] = data[5] * 256 + data[4];
	if(dig_T[2] > 32767)
		dig_T[2] -= 65536;
    dig_P[0] = data[7] * 256 + data[6];
    for (int i = 8, j = 1; i < 24; i += 2, ++j)
    {
        dig_P[j] = data[i + 1] * 256 + data[i];
        if (dig_P[j] > 32767)
            dig_P[j] -= 65536;
    }

    {
        uint8_t config = 0x27;
        if (i2c_reg_write_self(device_id, 0xF4, &config, 1))
            throw std::runtime_error("could not write config!");
    }
    {
        uint8_t config = 0xA0;
        if (i2c_reg_write_self(device_id, 0xF5, &config, 1))
            throw std::runtime_error("could not write config!");
    }
}

void Pressure::run() {
    uint8_t data[8] = {0};
    if (i2c_reg_read_self(device_id, 0xF7, data, 8))
        throw std::runtime_error("could not read data!");

    const long adc_p = (((long)data[0] * 65536) + ((long)data[1] * 256) + (long)(data[2] & 0xF0)) / 16;
    const long adc_t = (((long)data[3] * 65536) + ((long)data[4] * 256) + (long)(data[5] & 0xF0)) / 16;
    
    double var1 = (((double)adc_t) / 16384.0 - ((double)dig_T[0]) / 1024.0) * ((double)dig_T[1]);
    double var2 = ((((double)adc_t) / 131072.0 - ((double)dig_T[0]) / 8192.0) *(((double)adc_t)/131072.0 - ((double)dig_T[0])/8192.0)) * ((double)dig_T[2]);
    const double t_fine = (long)(var1 + var2);
    const double temperature = (var1 + var2) / 5120.0;
    
    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)dig_P[5]) / 32768.0;
    var2 = var2 + var1 * ((double)dig_P[4]) * 2.0;
    var2 = (var2 / 4.0) + (((double)dig_P[3]) * 65536.0);
    var1 = (((double) dig_P[2]) * var1 * var1 / 524288.0 + ((double) dig_P[1]) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((double)dig_P[0]);
    double p = 1048576.0 - (double)adc_p;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((double) dig_P[8]) * p * p / 2147483648.0;
    var2 = p * ((double) dig_P[7]) / 32768.0;
    const double pressure = (p + (var1 + var2 + ((double)dig_P[6])) / 16.0) / 100;
}

void Pressure::shutdown() {
    fd = -1;
}

int8_t Pressure::i2c_reg_write_self(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    if (device_id != i2c_addr)
        return 1;
    std::vector<uint8_t> data(length + 1, 0);
    data[0] = reg_addr;
    std::copy(reg_data, reg_data + length, data.begin() + 1);
    const int count = write(fd, data.data(), data.size());
    return count != static_cast<int>(data.size());
}

int8_t Pressure::i2c_reg_read_self(uint8_t i2c_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
    if (device_id != i2c_addr)
        return 1;
    int count = write(fd, &reg_addr, 1);
    if (count != 1)
        return 1;
    count = read(fd, reg_data, length);
    return count != length;
}
