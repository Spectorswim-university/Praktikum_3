#include <lcd.h>
#include "lcdisplay.h"
#include "wiringPi.h"
#include <stdexcept>


LCD::LCD()
{
}

void LCD::initialize()
{
    if (lcd = lcdInit(
        2, // two rows
        16, // 16 characters per row
        4, // 4-bit mode
        lcd_rs,
        lcd_e,
        lcd_d4,
        lcd_d5,
        lcd_d6,
        lcd_d7,
        0,
        0,
        0,
        0
        ) == -1)
        throw std::runtime_error("lcdInit failed!");

    lcdDisplay(lcd, true);
    lcdCursor(lcd, false);
    lcdCursorBlink(lcd, false);
    pos = 0;
}

void LCD::run(const char* text)
{
    if ((pos % 32) == 0)
    {
        // clear display
        lcdClear(lcd);
        lcdHome(lcd);
    }
    lcdClear(lcd);
    lcdPuts(lcd,text);
    delay(100);
}

void LCD::shutdown()
{
    // display off
    lcdDisplay(lcd, false);
    digitalWrite(lcd_d4, 0);
    digitalWrite(lcd_d5, 0);
    digitalWrite(lcd_d6, 0);
    digitalWrite(lcd_d7, 0);
    digitalWrite(lcd_e, 0);
    digitalWrite(lcd_rs, 0);
    digitalWrite(lcd_rw, 0);
}


