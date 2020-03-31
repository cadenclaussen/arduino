#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Library: https://github.com/johnrickman/LiquidCrystal_I2C
// Getting Strated: https://www.makerguides.com/character-i2c-lcd-arduino-tutorial
// Use i2c project to get the starting address
//
// Hardware:
// - SunStar 2004A LCD (20x4)
//
// LCD wiring:
// - Wire Arduino GND to GND
// - Wire Arduino 5V to VCC
// - Wire Arduino SDA-20 to SDA
// - Wire Arduino SCL-21 to SCL


// Connect to LCD via I2C (see i2c/i2c.ino): address 0x27
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);


void setup() {
    lcd.init();
    lcd.backlight();
}


void loop() {
    lcd.setCursor(0, 0);
    lcd.print("Hello World!");
    lcd.setCursor(0, 1);
    lcd.print("Line 2");
    lcd.setCursor(0, 2);
    lcd.print("Line 3");
    lcd.setCursor(0, 3);
    lcd.print("Line 4");

    lcd.setCursor(19, 0);
    lcd.print("W");
    lcd.setCursor(19, 1);
    lcd.print("X");
    lcd.setCursor(19, 2);
    lcd.print("Y");
    lcd.setCursor(19, 3);
    lcd.print("Z");
}
