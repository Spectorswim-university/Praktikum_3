#pragma once

#include <vector>
#include <cstdint>

class Stepper {
    public:
        Stepper();
        std::vector<int> pins;
        std::vector<uint8_t> halfstepping = { 
            0b1000,
            0b1100,
            0b0100,
            0b0110,
            0b0010,
            0b0011,
            0b0001,
            0b1001
        };
        uint32_t steps = 32;
        float reduction_gear = 63.68395f;
        int step;
        int direction;

        void initialize();
        void run();
        void shutdown();
};
