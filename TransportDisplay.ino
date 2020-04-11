#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// NodeMCU uses 0x3F as the address
// Set screen to width 16 and height 2
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup()
{
    lcd.begin();
    lcd.backlight();

    lcd.setCursor(0, 0);
    lcd.print("Establishing HSL");
    lcd.setCursor(0, 1);
    lcd.print("connection...");
}

void loop()
{
    
}
