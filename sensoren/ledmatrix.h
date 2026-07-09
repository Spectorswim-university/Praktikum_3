#pragma once
#include <cstdint>

// MAX7219
class LedMatrix 
{
public:
    LedMatrix();

    int clk;
    int din;
    int load;
    int pos;

    void initialize();
    void run(float distance);
    void shutdown(); 
    void send(uint8_t addr, uint8_t data) const;

};
