// LCD
// NOTE: Must download LiquidCrystal I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Temperature & Humidity Sensor
// NOTE: Must download both the DHT and Adafruit Sensor libraries
#include <DHT.h>

// Other
#include <math.h>


// LCD wiring:
// - Arduino GND to GND
// - Arduino 5V to VCC
// - Arduino SDA-20 to SDA
// - Arduino SCL-21 to SCL
//
// DHT22 wiring (power to ground):
// - 5V to slot 1
// - DHT22 in slot 1, 2, and 3
// - slot 3 to ground
// - slot 2 to Control-4


const int TEMPERATURE = 4;


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
DHT HT(TEMPERATURE, DHT22);


int humidity;
int temperature;


void setup() {
    Serial.begin(9600);

    // Initialize the LCD
    lcd.init();
    lcd.backlight();

    // Initialize the DHT22 temperature and humidity sensor
    HT.begin();

    // Initalization delay
    delay(2000);
}


void loop() {
    getTemperatureAndHumidity();
    print();
    delay(1000);
}


void print() {
    // Print 'Hello World!' on the first line of the LCD:
    lcd.setCursor(0, 0);
    lcd.print("Temperature:");
    lcd.setCursor(13, 0);
    lcd.print(temperature);
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.setCursor(13, 1);
    lcd.print(humidity);
}


void getTemperatureAndHumidity() {
    humidity = (int) round(HT.readHumidity());
    temperature = (int) round(HT.readTemperature(true));
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("F  Humidity: ");
    Serial.print(humidity);
    Serial.println();
}
