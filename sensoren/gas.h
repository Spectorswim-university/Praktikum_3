#pragma once

#include <cstdint>

class Gas {
    public:
        Gas();
        int fd;
        uint8_t device_id = 0x5A;

        void initialize();
        void run();
        void shutdown();
};
