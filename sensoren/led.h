#pragma once
#include <vector>

class Led{
    public:
        Led();
        std::vector<int> pins;
        void initialize();
        void run();
        void shutdown();
};
