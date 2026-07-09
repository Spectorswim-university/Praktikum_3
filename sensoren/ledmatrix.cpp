#include "ledmatrix.h"
#include <chrono>
#include <thread>
#include "wiringPi.h"
#include <cmath>
#include <iostream>

using namespace std::literals::chrono_literals;

LedMatrix::LedMatrix()
{
}

void LedMatrix::initialize()
{
    pinMode(clk, OUTPUT);
    pinMode(din, OUTPUT);
    pinMode(load, OUTPUT);
    digitalWrite(load, 1);

    // No decode for digits 7-0
    send(0x09, 0x00);

    // Intensity, 31/32
    send(0x0A, 0x0F);

    // Scan Limit, all digits
    send(0x0B, 0x07);

    // Shutdown, Normal Operation
    send(0x0C, 0x01);

    // Test Mode
    send(0x0F, 0x01);
    delay(500);
    send(0x0F, 0x00);

    pos = 64;
}

void LedMatrix::run(float distance)
{
    // origin: top, left
    // y-axis: digits (0, ..., 7)
    // x-axis: segments (DP, A, ..., G)
	
    // fill maxtrix dot by dot, row-wise
    
    //
    
    for (int y = 1; y <= 8; ++y) {
    	send(y, 0);
    }

    int column = floor(distance);

    if (column > 8)
	    column = 8;

    send(column, 255 << (8 - column));
    
    delay(1000 / 64);
    /*    
    const int y = pos / 8;
    const int x = pos % 8;
    const int val = 256 - static_cast<int>(128.0f / (1 << x));
    std::cout << "val = " << val << std::endl;
    send(y + 1, val);
    delay(1000 / 64);
    ++pos;
    */
}

void LedMatrix::shutdown()
{
    // Shutdown
    send(0x0C, 0x00);

    digitalWrite(clk, 0);
    digitalWrite(din, 0);
    digitalWrite(load, 0);
}


void LedMatrix::send(uint8_t addr, uint8_t data) const
{
    // 16-bit shift register
    // 4 bit ignored
    // 4-bit address
    // 8-bit data
    const uint16_t word = (static_cast<uint16_t>(addr) << 8) | data;

    digitalWrite(load, 0);
    for (int b = 15; b >= 0; --b)
    {
        digitalWrite(din, word & (1 << b));
        std::this_thread::sleep_for(25ns);
        digitalWrite(clk, 1);
        std::this_thread::sleep_for(50ns);
        digitalWrite(clk, 0);
        std::this_thread::sleep_for(50ns);
    }
    digitalWrite(load, 1); // latch
    std::this_thread::sleep_for(50ns);
}

