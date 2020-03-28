#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <math.h>
#include <Keypad.h>


// Board requirements
//
// Total:               4 PWM Pins, 15 Digital Pins
//
// LED:                 3 Pin(s) (ideally PWM)
// DHT22 Temp/Humidity: 1 Pin(s) (PWM)
// Buttons:             3 Pin(s)
// Keypad:              8 Pin(s)
// MicroSD:             4 Pin(s)
// LCD: SCL/SCA
// DS3231 Date/Time: SCL/SCA


// LED
const int BLUE_LED_PIN = 2;
const int GREEN_LED_PIN = 3;
const int RED_LED_PIN = 4;

// Buttons
const int GREEN_BUTTON_PIN = 5;
const int RED_BUTTON_PIN = 6;
const int BLACK_BUTTON_PIN = 7;
int priorButtonValue[8];

// Temperature and Humidity Sensor
const int DHT22_PIN = 8;
DHT dht22(DHT22_PIN, DHT22);
int temperature;
int humidity;

// LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);

// Date/Time Sensor
RTC_DS3231 ds3231;
char dateTime[30];

// Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {{ '1', '2', '3', 'A' },
                         { '4', '5', '6', 'B' },
                         { '7', '8', '9', 'C' },
                         { '*', '0', '#', 'D' }};
byte rowPins[ROWS] = { 38, 39, 40, 41 };
byte colPins[COLS] = { 42, 43, 44, 45 };
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// States of the application
int state;
const int STOPPED = 1;
const int RECORDING = 2;
const int PAUSED = 3;


int cadenGuess;
int shaneGuess;
char guesses[5];


int loopDelay = 50;
int aggregateLoopDelay = 0;


uint32_t startTime;
uint32_t endTime;
uint32_t elapsedMilliseconds;
char elapsedTime[19];

uint32_t pauseStartTime;
uint32_t aggregatePauseTime;


boolean justStarted = false;
boolean justStopped = false;
boolean justCancelled = false;
boolean justPaused = false;


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
    } else {
        ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Initialize the DHT22 temperature and humidity sensor
    dht22.begin();

    // Initialize the buttons
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
    pinMode(BLACK_BUTTON_PIN, INPUT_PULLUP);

    // Initialize the LED
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    // Initialize the buttons to unpressed
    priorButtonValue[BLUE_LED_PIN] = HIGH;
    priorButtonValue[GREEN_LED_PIN] = HIGH;
    priorButtonValue[RED_LED_PIN] = HIGH;

    // Set the state to stopped
    state = STOPPED;
    red();

    // Initial delay
    delay(500);
}


void loop() {


    switch (state) {


    case STOPPED:
        if (buttonJustPressed(GREEN_BUTTON_PIN)) {
            startRecording();
        }

        if (justStopped) {
            if (aggregateLoopDelay % 1000 < 500) {
                red(); // red
            } else {
                off(); // off
            }
        } else if (justCancelled) {
            if (aggregateLoopDelay % 1000 < 500) {
                blue(); // blue
            } else {
                off(); // off
            }
        } else {
            red(); // red
        }
        break;


    case RECORDING:

        // Transition if required
        if (buttonJustPressed(GREEN_BUTTON_PIN)) {
            pauseRecording();
        } else if (buttonJustPressed(BLACK_BUTTON_PIN)) {
            cancelRecording();
        } else if (buttonJustPressed(RED_BUTTON_PIN)) {
            summarizeRecording();
        } else {

            // Still in the recording state...
            if (aggregateLoopDelay % 1000 == 0) {
                getElapsedTime();
            }

            if (justStarted) {
                if (aggregateLoopDelay % 1000 < 500) {
                    green();
                } else {
                    off();
                }
            } else {
                green();
            }
        }

        break;


    case PAUSED:

        // Transition if required
        if (buttonJustPressed(GREEN_BUTTON_PIN)) {
            continueRecording();
        } else if (buttonJustPressed(BLACK_BUTTON_PIN)) {
            continueRecording();
            cancelRecording();
        } else if (buttonJustPressed(RED_BUTTON_PIN)) {
            continueRecording();
            summarizeRecording();
        } else {
            // Still in the paused state...

            // This code effectively blinks the LED between off and
            // yellow, being in the off position for the first half of
            // a second and then on/yellow for the second half of the
            // second, continuously.
            if (justPaused) {
                if (aggregateLoopDelay % 1000 < 500) {
                    yellow();
                } else {
                    off();
                }
            } else {
                yellow();
            }
        }
        break;
    }


    // Get the time ever second and the temperature and
    // humdity every 5 seconds
    if (aggregateLoopDelay % 1000 == 0) {
        getDateTime();
        if (aggregateLoopDelay % 5000 == 0) {
            getTemperatureHumidity();
        }
        updateLcd();
    }


    delay(loopDelay);
    aggregateLoopDelay += loopDelay;
    if (aggregateLoopDelay > 5000) {
        aggregateLoopDelay = 0;
        justStarted = false;
        justStopped = false;
        justCancelled = false;
        justPaused = false;
    }
}


// Did the button @ buttonPin just get pressed in this loop?
boolean buttonJustPressed(int buttonPin) {

    // Read the current value of the button
    int buttonValue = digitalRead(buttonPin);

    // The button was previously up, and is still up, it is unchanged
    if (buttonValue == HIGH && priorButtonValue[buttonPin] == HIGH) {
        return false;
    }

    // The button was previously down, and is still down, it is unchanged
    if (buttonValue == LOW && priorButtonValue[buttonPin] == LOW) {
        return false;
    }

    // The button was just released (back in the up position)
    if (buttonValue == HIGH && priorButtonValue[buttonPin] == LOW) {
        priorButtonValue[buttonPin] = HIGH;
        return false;
    }

    // The button was just pressed (back in the down position)
    if (buttonValue == LOW && priorButtonValue[buttonPin] == HIGH) {
        priorButtonValue[buttonPin] = LOW;
        return true;
    }
}


void startRecording() {
    Serial.println("Recording started...");
    state = RECORDING;
    startTime = millis();
    aggregatePauseTime = 0;
    pauseStartTime = 0;
    justStarted = true;
    aggregateLoopDelay = 0;
}


void pauseRecording() {
    Serial.println("Recording paused...");
    state = PAUSED;
    pauseStartTime = millis();
    justPaused = true;
    aggregateLoopDelay = 0;
}


void continueRecording() {
    Serial.println("Recording continued...");
    state = RECORDING;
    uint32_t pauseEndTime = millis();
    aggregatePauseTime += pauseEndTime - pauseStartTime;
    justStarted = true;
    aggregateLoopDelay = 0;
}


void cancelRecording() {
    Serial.println("Recording cancelled...");
    state = STOPPED;
    justCancelled = true;
    aggregateLoopDelay = 0;
}


void summarizeRecording() {
    Serial.println("Recording summarization...");
    state = STOPPED;
    // Write information to the MicroSD Cardg
    justStopped = true;
    aggregateLoopDelay = 0;
}


void updateLcd() {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(dateTime);

    lcd.setCursor(0, 1);
    lcd.print(temperature);
    lcd.print((char) 223);

    lcd.setCursor(4, 1);
    lcd.print(humidity);

    if (state > STOPPED) {
        lcd.setCursor(0, 2);
        lcd.print(elapsedTime);
    }
}


void off() {
    led(LOW, LOW, LOW);
}


void red() {
    led(HIGH, LOW, LOW);
}


void green() {
    led(LOW, HIGH, LOW);
}


void blue() {
    led(LOW, LOW, HIGH);
}


void yellow() {
    led(HIGH, HIGH, LOW);
}


void led(int red, int green, int blue) {
    if (red == LOW || red == HIGH) {
        digitalWrite(RED_LED_PIN, red);
    } else {
        analogWrite(RED_LED_PIN, red);
    }

    if (green == LOW || green == HIGH) {
        digitalWrite(GREEN_LED_PIN, green);
    } else {
        analogWrite(GREEN_LED_PIN, green);
    }

    if (blue == LOW || blue == HIGH) {
        digitalWrite(BLUE_LED_PIN, blue);
    } else {
        analogWrite(BLUE_LED_PIN, blue);
    }
}


void getDateTime() {
    DateTime now = ds3231.now();
    char daysOfTheWeek[7][12] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    int hour = now.hour();
    char ampm = 'a';
    if (hour > 12) {
        hour -= 12;
        ampm = 'p';
    }
    sprintf(dateTime, "%s %02d/%02d %02d:%02d:%02d%c", daysOfTheWeek[now.dayOfTheWeek()], now.month(), now.day(), hour, now.minute(), now.second(), ampm);
}


void getTemperatureHumidity() {
    temperature = (int) round(dht22.readTemperature(true));
    humidity = (int) round(dht22.readHumidity());
}


void getElapsedTime() {
    elapsedMilliseconds = millis();
    int minutes = ((elapsedMilliseconds - startTime - aggregatePauseTime) / 1000) / 60;
    int seconds = ((elapsedMilliseconds - startTime - aggregatePauseTime) / 1000) % 60;
    sprintf(elapsedTime, "%02d:%02d", minutes, seconds);
}
