#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Library: https://github.com/johnrickman/LiquidCrystal_I2C
// Getting Strated: https://www.makerguides.com/character-i2c-lcd-arduino-tutorial
//
// Hardware:
// - SunStar 2004A LCD (20x4)
//
// LCD wiring:
// - Arduino GND to GND
// - Arduino 5V to VCC
// - Arduino SDA-20 to SDA
// - Arduino SCL-21 to SCL


// Wiring: SDA pin is connected to A4 and SCL pin to A5.
// Connect to LCD via I2C (see i2c/i2c.ino): address 0x27
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);


void setup() {
    lcd.init();
    lcd.backlight();
}


void loop() {
    // Print 'Hello World!' on the first line of the LCD:
    lcd.setCursor(0, 0);
    lcd.print("Hello World!");
    lcd.setCursor(2, 1);
    lcd.print("LCD tutorial");
}
