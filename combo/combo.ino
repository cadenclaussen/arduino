// LCD
// NOTE: Must download LiquidCrystal I2C
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// DS3231: Time
#include "RTClib.h"


// DHT22: Temperature & Humidity Sensor
// NOTE: Must download both the DHT and Adafruit Sensor libraries
#include <DHT.h>


// Other
#include <math.h>


// Common Cathode LED wiring:
// - Place common cathode LED into the breadboard in slots 1 (blue), 2 (green), 3 (ground), and 4 (red)
// - Wire Arduino PWM 2 to slot 1 (blue)
// - Wire Arduino PWM 3 to slot 2 (green)
// - Wire Arduino GND   to slot 3 (ground)
// - Wire Arduino PWM 4 to slot 4 (red)
// - Add 330 ohm resisters in slots 1, 2, and 4 between the wire from
//   the Arduino PWM and the Common Cathode's pins
//
// DS3132 wiring
// - Arduino GND to GND
// - Arduino 5V to VDD
// - Arduino SDA-20 to SDA (or shared SDA breadboard column)
// - Arduino SCL-21 to SCL (or shared SCL breadboard column)
//
// BUTTON wiring (power to ground):
// - POWER: Arduino 5V to breadboard column 10
// - 10K ohm resister from breadboard column 10 to breadboard column 11
// - Wire from breadboard column 11 to breadboard column 12
// - button in breadboard column 12 (positive) and breadboard column 14 (negative)
// - GROUND: breadboard column 14 to Arduino GND
// - CONTROL: breadboard column 11 to Arduino 3 (should be on the outer portion of the circuit)
//
// LCD wiring:
// - Arduino GND to GND
// - Arduino 5V to VCC
// - Arduino SDA-20 to SDA (or shared SDA breadboard column)
// - Arduino SCL-21 to SCL (or shared SCL breadboard column)


const int LED_BLUE = 2;
const int LED_GREEN = 3;
const int LED_RED = 4;

const int BUTTON_RECORD_PAUSE = 5;
const int BUTTON_STOP = 6;


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
RTC_DS3231 ds3231;
DHT dht22(TEMPERATURE, DHT22);


String date;
String time;
int temperature;
int humidity;
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
    dht22.begin();

    // Initialize the button
    pinMode(BUTTON_RECORD_PAUSE, INPUT);

    // Initialize the LED
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);

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
    int buttonState = digitalRead(BUTTON_RECORD_PAUSE);
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
}


void getTemperature() {
    int tempC = ds3231.getTemperature();
    temperature = (tempC * 1.8) + 32;
}


void getTemperatureHumidity() {
    humidity = (int) round(dht22.readHumidity());
    temperature = (int) round(dht22.readTemperature(true));
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
