#pragma once

#include <vector> 
 
class Rgb {
    public:
        Rgb();
        float hue;
        int period;
        std::vector<int> pins;
        void initialize();
        void run();
        void shutdown();

};
