// Bewegungsmelder

class Movement 
{
    public:
        Movement();
        int pin;
        void initialize();
        int run();
        void shutdown();
};
