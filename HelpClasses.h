#pragma once

#include <wiringPi.h> 
#include <string>
#include "sensoren/lcdisplay.h"
#include "sensoren/seg7.h"
#include "sensoren/ledmatrix.h"
#include "sensoren/dht11.h"

class LED
{
    private:
        int pinRED, pinYELLOW, pinGREEN;
    public:
        LED(int a_pinRED, int a_pinYELLOW, int a_pinGREEN)
            :pinRED(a_pinRED), pinYELLOW(a_pinYELLOW), pinGREEN(a_pinGREEN)
        {
            wiringPiSetup();

            pinMode(pinRED, OUTPUT);
            pinMode(pinYELLOW, OUTPUT);
            pinMode(pinGREEN, OUTPUT);

            digitalWrite(pinRED, LOW);
            digitalWrite(pinYELLOW, LOW);
            digitalWrite(pinGREEN, LOW);
        }
        
        void RED()
        {
            digitalWrite(pinRED, HIGH);
            digitalWrite(pinYELLOW, LOW);
            digitalWrite(pinGREEN, LOW);
        }
        
        void YELLOW()
        {
            digitalWrite(pinRED, LOW);
            digitalWrite(pinYELLOW, HIGH);
            digitalWrite(pinGREEN, LOW);
        }
        
        void GREEN()
        {
            digitalWrite(pinRED, LOW);
            digitalWrite(pinYELLOW, LOW);
            digitalWrite(pinGREEN, HIGH);
        }

        void OFF()
        {
            digitalWrite(pinRED, LOW);
            digitalWrite(pinYELLOW, LOW);
            digitalWrite(pinGREEN, LOW);
        }

};

class Display
{
    public:
        enum DisplayType
        {
            SEG7,
            LCDisplay,
            LEDMatrix
        } DT;

        Display(DisplayType a_DT)
        : DT(a_DT)
        {
            switch (DT)
            {
                case SEG7:
                    seg7.din = 27; seg7.load = 28; seg7.clk = 29;
                    seg7.initialize();
                    break;
                case LCDisplay:
                    lcd.lcd_d4 = 6; lcd.lcd_d5 = 10; lcd.lcd_d6 = 11; lcd.lcd_d7 = 31;
                    lcd.lcd_e = 26; lcd.lcd_rs = 4; lcd.lcd_rw = 5;
                    lcd.initialize();
                    break;
                case LEDMatrix:
                    ledMatrix.din = 27; ledMatrix.load = 28; ledMatrix.clk = 29;
                    ledMatrix.initialize();
                    break;
            }
        }

        void print(float a_temperatur)
        {
            switch (DT)
            {
                case SEG7:
                    printSEG7(a_temperatur);
                    break;
                case LCDisplay:
                    printLCD(a_temperatur);
                    break;
                case LEDMatrix:
                    printLEDMatrix(a_temperatur);
                    break;
            }
        }

        void shutdown()
        {
            switch (DT)
            {
                case SEG7:
                    seg7.shutdown();
                    break;
                case LCDisplay:
                    lcd.shutdown();
                    break;
                case LEDMatrix:
                    ledMatrix.shutdown();
                    break;
            }
        }
    private:
        Seg7 seg7;
        LCD lcd;
        LedMatrix ledMatrix;


        void printSEG7(float a_temperatur)
        {
            std::string text = std::to_string(a_temperatur).substr(0, 4);
            int addr = 8;
            for (size_t i = 0; i < text.length(); ++i)
            {
                uint8_t c = text[i] - '0';
                if(i + 1 < text.length() && text[i + 1] == '.')
                {
                    c += 0x80;
                    ++i;
                }
                seg7.write(addr, c);
                --addr;
            }
        }

        void printLCD(float a_temperatur)
        {
            std::string str = std::to_string(a_temperatur);
            lcd.run(str.c_str());
        }

        void printLEDMatrix(float a_temperatur)
        {
            float distance = (a_temperatur - 15.0) / 2;
            ledMatrix.run(distance);
        }

};

class Sensoren
{
    private:
        Dht dht;
        int pin;
    public:
        Sensoren(int a_pin = 30)
        : pin(a_pin)
        {
            //
        }

        void read(float &a_humidity, float &a_temperature)
        {
            auto [temperatur, humidity] = dht.Run(pin);
            a_temperature = temperatur;
            a_humidity = humidity;
        }

};