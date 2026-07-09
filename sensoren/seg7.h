#include <cstdint>
#include <string>

// MAX7219
class Seg7 {
    public:
        Seg7();
        std::string pi;
        int clk;
        int din;
        int load;
        void run();
        void initialize();
        void shutdown();
        void write(uint8_t addr, uint8_t data) const;
    private:
        int digit;
};
