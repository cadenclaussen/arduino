// LCD
// NOTE: Must download LiquidCrystal I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// DS3231: Time
#include "RTClib.h"


// DT22: Temperature & Humidity Sensor
// NOTE: Must download both the DHT and Adafruit Sensor libraries
#include <DHT.h>


// Other
#include <math.h>


// LCD wiring:
// - Arduino GND to GND
// - Arduino 5V to VCC
// - Arduino SDA-20 to SDA (or shared SDA breadboard column)
// - Arduino SCL-21 to SCL (or shared SCL breadboard column)
//
// DS3132 wiring
// - Arduino GND to GND
// - Arduino 5V to VDD
// - Arduino SDA-20 to SDA (or shared SDA breadboard column)
// - Arduino SCL-21 to SCL (or shared SCL breadboard column)
//
// DHT22 wiring (power to ground):
// - 5V to breadboard column 1
// - DHT22 in breadboard column 1, 2, and 3
// - breadboard column 3 to ground
// - breadboard column 2 to Arduino 4
//
// LED wiring (power to ground):
// - POWER/CONTROL: Arduino 2 to breadboard column 1
// - Both ends of 330 ohm resister in breadboard column 1 between Arduino 2 connection and LED
// - LED positive in breadboard column 1 (long end of the LED)
// - LED negative in breadboard column 2 (short end of the LED)
// - GROUND: breadboard column 2 to Arduino GND
//
// BUTTON wiring (power to ground):
// - POWER: Arduino 5V to breadboard column 10
// - 10K ohm resister from breadboard column 10 to breadboard column 11
// - Wire from breadboard column 11 to breadboard column 12
// - button in breadboard column 12 (positive) and breadboard column 14 (negative)
// - GROUND: breadboard column 14 to Arduino GND
// - CONTROL: breadboard column 11 to Arduino 3 (should be on the outer portion of the circuit)


const int LED = 2;
const int BUTTON = 3;
const int TEMPERATURE = 4;


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
RTC_DS3231 ds3231;
DHT HT(TEMPERATURE, DHT22);


int humidity;
int temperature;
String date;
String time;
boolean timerStarted;


void setup() {
    Serial.begin(9600);

    // Initialize the LCD
    lcd.init();
    lcd.backlight();

    // Initialize rtc library
    ds3231.begin();
    if (ds3231.lostPower()) {
        // Use compilation date/time to reset
        Serial.println("RTC lost power.");
        ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Initialize the DHT22 temperature and humidity sensor
    HT.begin();

    // Initialize the button
    pinMode(BUTTON, INPUT);

    // Initialize the LED
    pinMode(LED, OUTPUT);

    // Inital delay
    delay(500);
}


void loop() {
    getTimer();
    getDateTime();
    getTemperatureHumidity();
    updateLcd();
    delay(1000);
}


void getTimer() {
    int buttonState = digitalRead(BUTTON);
    if (buttonState == LOW) {
        if (timerStarted == false) {
            digitalWrite(LED, HIGH);
            timerStarted = true;
        } else {
            digitalWrite(LED, LOW);
            timerStarted = false;
        }
    }
}


void getDateTime() {
    DateTime now = ds3231.now();
    char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    // Determine date
    String dow = daysOfTheWeek[now.dayOfTheWeek()];
    String month = (String) now.month();
    String day = (String) now.day();
    String year = (String) now.year();
    date = dow + " " + month + "/" + day + "/" + year;

    // Determine time
    String hour = (String) now.hour();
    String minute = (String) now.minute();
    String second = (String) now.second();
    time = hour + ":" + minute + ":" + second;

    Serial.println(date + " " + time);
}


void getTemperatureHumidity() {
    humidity = (int) round(HT.readHumidity());
    temperature = (int) round(HT.readTemperature(true));
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("F  Humidity: ");
    Serial.print(humidity);
    Serial.println();
}


void updateLcd() {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(date);

    lcd.setCursor(0, 1);
    lcd.print(time);

    lcd.setCursor(0, 2);
    lcd.print(temperature);
    lcd.setCursor(3, 2);
    lcd.print("degrees");

    lcd.setCursor(0, 3);
    lcd.print("Humidity");
    lcd.setCursor(9, 3);
    lcd.print(humidity);
}
