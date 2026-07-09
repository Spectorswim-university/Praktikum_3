#pragma once

#include <vector>


class Buzzer {
    public:
        Buzzer();
        int pin;
        std::vector<int> melody;
        void initialize();
        void run();
        void shutdown();
};
