#pragma once

#include <string>
#include <lcd.h>

// HD44780U
class LCD  
{
public:
    LCD();

    int lcd_d4;
    int lcd_d5;
    int lcd_d6;
    int lcd_d7;
    int lcd_e;
    int lcd_rs;
    int lcd_rw;
    int pos;
    int lcd;

    void initialize();
    void run(const char*);
    void shutdown();
};

