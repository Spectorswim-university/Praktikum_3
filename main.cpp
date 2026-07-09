#include <wiringPi.h> 
#include <semaphore>
#include <thread>
#include <atomic>
#include <chrono>
#include "HelpClasses.h"

using sem1 = std::counting_semaphore<1>;
using sem2 = std::counting_semaphore<2>;

void thread1(sem2 &s1, sem1 &s2, sem1 &s3, float &a_humidity, float &a_temperature, std::atomic<bool> &a_stop)
{
    Sensoren sensoren;
    while (!a_stop)
    {
        s1.acquire();
        s1.acquire();
        
        sensoren.read(a_humidity, a_temperature);

        s2.release();
        s3.release();
    }
    s2.release();
    s3.release();

}

void thread2(sem2 &s1, sem1 &s2, sem1 &s3, float &a_temperature, std::atomic<bool> &a_stop)
{
    LED led(0, 2, 3);
    while (!a_stop)
    {
        s2.acquire();

        if(a_temperature < 20.0)
        {
            led.YELLOW();
        }
        else if(a_temperature < 30.0)
        {
            led.GREEN();
        }
        else
        {
            led.RED();
        }

        s1.release();
    }
    led.OFF();

}

void thread3(sem2 &s1, sem1 &s2, sem1 &s3, float &a_temperature, std::atomic<bool> &a_stop)
{
    Display display(Display::DisplayType::SEG7);
    while (!a_stop)
    {
        s3.acquire();
        
        display.print(a_temperature);

        s1.release();
    }
    display.shutdown();
    
}

int main()
{
    wiringPiSetup();
    sem2 s1{2};
    sem1 s2{0}, s3{0};
    float humidity, temperature;
    std::atomic<bool> stop{false};

    std::thread t1([&]() 
        { 
            thread1(s1, s2, s3, humidity, temperature, stop); 
        }
    );
    std::thread t2([&]() 
        { 
            thread2(s1, s2, s3, temperature, stop); 
        }
    );
    std::thread t3([&]() 
        { 
            thread3(s1, s2, s3, temperature, stop); 
        }
    );

    std::this_thread::sleep_for(std::chrono::seconds(15));
    stop = true;
    t1.join();
    t2.join();
    t3.join();


}