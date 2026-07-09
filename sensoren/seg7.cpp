#include "seg7.h"
#include <thread>
#include <chrono>
#include <wiringPi.h>
#include <array>
#include <atomic>
#include <iostream>

using namespace std::literals::chrono_literals;


Seg7::Seg7() {
Seg7::pi = "3141592653589793238462643383279502884197169399375105820974944592307816406286";
}

void Seg7::initialize()
{

    std::cout <<"Seg7::Initialize()" <<std::endl;
    pinMode(clk, OUTPUT);
    pinMode(din, OUTPUT);
    pinMode(load, OUTPUT);
    digitalWrite(load, 1);

    // Code B decode for digits 7-0
    write(0x09, 0xFF);
    std::this_thread::sleep_for(1000ms);

    // Intensity, 15/32
    write(0x0A, 0x0F);

    // Scan Limit, all digits
    write(0x0B, 0x07);

    // Shutdown, Normal Operation
    write(0x0C, 0x01);

    // Test Mode
    write(0x0F, 0x01);
    delay(500);
    write(0x0F, 0x00);

    digit = 0;
}

void Seg7::run()
{
    // scroll digits of pi, including decimal point, right to left, filled with blanks
    for (int d = 7; d >= 0; --d)
    {
        // no ASCII!
        // decimal point set by D7=1
        if (digit - d < 0 || digit - d >= static_cast<int>(pi.size()))
            write(d + 1, 0x0F); // blank
        else
            write(d + 1, pi.at(digit - d) - '0' + (digit == d ? 0x80 : 0));
    }
    delay(200);

    digit = (digit + 1) % pi.size();
}

void Seg7::shutdown()
{
    std::cout <<"Seg7::Shutdown()" <<std::endl;
    // Shutdown
    write(0x0C, 0x00);

    digitalWrite(clk, 0);
    digitalWrite(din, 0);
    digitalWrite(load, 0);
}


void Seg7::write(uint8_t addr, uint8_t data) const
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
