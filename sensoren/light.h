#pragma once
#include <cstdint>

class Light {
    public:
        Light();
        uint8_t device_id = 0b0100011;
        int fd;
        void initialize();
        void run();
        void shutdown();
};
