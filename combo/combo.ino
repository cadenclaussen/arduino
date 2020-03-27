#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <math.h>


// LED PWM Pins
const int BLUE_LED_PIN = 2;
const int GREEN_LED_PIN = 3;
const int RED_LED_PIN = 4;

// Button PWM Pins
const int GREEN_BUTTON_PIN = 5;
const int RED_BUTTON_PIN = 6;
const int BLACK_BUTTON_PIN = 7;

// Temperature and Humidity
const int DHT22_PIN = 8;


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
RTC_DS3231 ds3231;
DHT dht22(DHT22_PIN, DHT22);


char dateTime[30];
int temperature;
int humidity;


int state;

const int STOPPED = 1;
const int RECORDING = 2;
const int PAUSED = 3;

int priorButtonValue[8];

char guesses[5];
int cadenGuess;
int shaneGuess;

int loopDelay = 50;
int aggregateLoopDelay = 0;

uint32_t elapsedMilliseconds;
uint32_t startTime;
uint32_t endTime;
uint32_t aggregatePauseTime;
uint32_t pauseStartTime;
char elapsedTime[19];

boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];


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
    ledRed();

    // Initial delay
    delay(500);
}


void loop() {




    switch (state) {


    case STOPPED:
        if (buttonPressed(GREEN_BUTTON_PIN)) {
            startRecording();
        }

        if (justStopped) {
            if (aggregateLoopDelay % 1000 < 500) {
                ledRed();
            } else {
                ledOff();
            }
        } else if (justCancelled) {
            if (aggregateLoopDelay % 1000 < 500) {
                ledBlue();
            } else {
                ledOff();
            }
        } else {
            ledRed();
        }
        break;


    case RECORDING:

        // Transition if required
        if (buttonPressed(GREEN_BUTTON_PIN)) {
            pauseRecording();
        } else if (buttonPressed(BLACK_BUTTON_PIN)) {
            cancelRecording();
        } else if (buttonPressed(RED_BUTTON_PIN)) {
            summarizeRecording();
        } else {

            // Still in the recording state...
            if (aggregateLoopDelay % 1000 == 0) {
                getElapsedTime();
            }

            if (justStarted) {
                if (aggregateLoopDelay % 1000 < 500) {
                    ledGreen();
                } else {
                    ledOff();
                }
            } else {
                ledGreen();
            }
        }

        break;


    case PAUSED:

        // Transition if required
        if (buttonPressed(GREEN_BUTTON_PIN)) {
            continueRecording();
        } else if (buttonPressed(BLACK_BUTTON_PIN)) {
            continueRecording();
            cancelRecording();
        } else if (buttonPressed(RED_BUTTON_PIN)) {
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
                    ledYellow();
                } else {
                    ledOff();
                }
            } else {
                ledYellow();
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


boolean buttonPressed(int buttonPin) {
    int buttonValue = digitalRead(buttonPin);

    // status unchanged:
    // - was previously up, still up, or
    // - was previously down, and still down
    if (buttonValue == priorButtonValue[buttonPin]) {
        return false;
    }

    // was just released (back in the up position)
    if (buttonValue == HIGH) {
        priorButtonValue[buttonPin] = HIGH;
        return false;
    }

    // was just pressed down
    priorButtonValue[buttonPin] = LOW;
    return true;
}


void startRecording() {
    Serial.println("Recording started...");
    state = RECORDING;
    ledGreen();
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
    ledGreen();
    uint32_t pauseEndTime = millis();
    aggregatePauseTime += pauseEndTime - pauseStartTime;
    justStarted = true;
    aggregateLoopDelay = 0;
}


void cancelRecording() {
    Serial.println("Recording cancelled...");
    state = STOPPED;
    ledRed();
    justCancelled = true;
    aggregateLoopDelay = 0;
}


void summarizeRecording() {
    Serial.println("Recording summarization...");
    state = STOPPED;
    ledRed();
    justStopped = true;
    aggregateLoopDelay = 0;
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

    /* Serial.print("Temperature: "); */
    /* Serial.print(temperature); */
    /* Serial.print(" Humidity: "); */
    /* Serial.println(humidity); */
}


void getElapsedTime() {
    elapsedMilliseconds = millis();
    int minutes = ((elapsedMilliseconds - startTime - aggregatePauseTime) / 1000) / 60;
    int seconds = ((elapsedMilliseconds - startTime - aggregatePauseTime) / 1000) % 60;
    sprintf(elapsedTime, "%02d:%02d", minutes, seconds);

    /* Serial.println(elapsedTime); */
}


void updateLcd() {
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(dateTime);

    lcd.setCursor(0, 1);
    lcd.print(temperature);
    lcd.print((char) 223);
    /* lcd.print(" "); */

    lcd.setCursor(4, 1);
    lcd.print(humidity);

    if (state > STOPPED) {
        lcd.setCursor(0, 2);
        lcd.print(elapsedTime);
    }
}


void ledOff() {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);
}


void ledOn() {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(BLUE_LED_PIN, HIGH);
}


void ledRed() {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);
}


void ledBlue() {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, HIGH);
}


void ledGreen() {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(BLUE_LED_PIN, LOW);
}


void ledYellow() {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(BLUE_LED_PIN, LOW);
}
