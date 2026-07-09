// HC-SR04

class Sonic 
{
    public:
        Sonic();
        int trig;
        int echo;
        void initialize();
        float run();
        void shutdown();
};
