#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <tuple>

////read return flag of sensor
#define DHTLIB_OK               0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2
#define DHTLIB_INVALID_VALUE    -999

#define DHTLIB_DHT11_WAKEUP     20
#define DHTLIB_DHT_WAKEUP       1

#define DHTLIB_TIMEOUT          100

class Dht{      
    public:
        Dht();
        float humidity;
        float temperature;    //use to store temperature and humidity data read
        int readDHT11Once(int pin);     //read DHT11
        int readDHT11(int pin);     //read DHT11
        std::tuple<float, float>Run(int pin);
    private:
        uint8_t bits[5];    //Buffer to receiver data
        int readSensor(int pin,int wakeupDelay);   

};
