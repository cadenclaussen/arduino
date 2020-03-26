#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>
#include <math.h>


const int LED_BLUE_PIN = 2;
const int LED_GREEN_PIN = 3;
const int LED_RED_PIN = 4;

const int BUTTON_RECORD_PAUSE_PIN = 5;
const int BUTTON_STOP_PIN = 6;
const int BUTTON_CANCEL_PIN = 7;

const int DHT22_PIN = 8;


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
RTC_DS3231 ds3231;
DHT dht22(DHT22_PIN, DHT22);


char dateTime[19];
int temperature;
int humidity;

int state;
const int INIT = 0;
const int STOPPED = 1;
const int RECORDING = 2;
const int PAUSED = 3;

int buttonRecordPauseLastState = HIGH;

char guesses[5];
int cadenGuess;
int shaneGuess;

int loopDelay = 50;
int lcdLoopDelay = 0;
int stateLoopDelay = 0;

uint32_t elapsedMilliseconds;
uint32_t startTime;
uint32_t endTime;
uint32_t aggregatePauseTime;
uint32_t pauseStartTime;
char elapsedTime[19];

boolean newData = false;
const byte numChars = 32;
char receivedChars[numChars];


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
    pinMode(BUTTON_RECORD_PAUSE_PIN, INPUT_PULLUP);
    pinMode(BUTTON_STOP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_CANCEL_PIN, INPUT_PULLUP);

    // Initialize the LED
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    initializeState();

    // Initial delay
    delay(500);

    Serial.print("SC> ");

    state = INIT;
    /* startRecording(); */
}


void loop() {


    if (state == INIT) {

        Serial.print("SC> ");
        while (Serial.available() < 2) {
        }
        shaneGuess = Serial.parseInt();
        Serial.println(shaneGuess);


        Serial.print("CC> ");
        while (Serial.available() < 2) {
        }
        cadenGuess = Serial.parseInt();
        Serial.println(cadenGuess);

        sprintf(guesses, "%02d/%02d", shaneGuess, cadenGuess);
        Serial.println(guesses);

        state = STOPPED;

    } else {
        getButtons();
    }


    getDateTime();
    delay(loopDelay);


    lcdLoopDelay += loopDelay;
    if (lcdLoopDelay > 1000) {
        lcdLoopDelay = 0;
        getElapsedTime();
        getTemperatureHumidity();
        updateLcd();
    }


    if (state == PAUSED) {
        stateLoopDelay += loopDelay;
        if (stateLoopDelay < 250) {
            ledOff();
        } else if (stateLoopDelay < 500) {
            ledYellow();
        } else {
            stateLoopDelay = 0;
        }
    } else {
        stateLoopDelay = 0;
    }
}


void getButtons() {
    int buttonRecordPause = digitalRead(BUTTON_RECORD_PAUSE_PIN);
    if (buttonRecordPause == LOW) {
        if (buttonRecordPauseLastState == HIGH) {
            if (state == STOPPED) {
                startRecording();
            } else if (state == RECORDING) {
                pauseRecording();
            } else if (state == PAUSED) {
                continueRecording();
            }
            buttonRecordPauseLastState = LOW;
        }
    } else {
        buttonRecordPauseLastState = HIGH;
    }

    int buttonStop = digitalRead(BUTTON_STOP_PIN);
    if (buttonStop == LOW) {
        if (state == PAUSED) {
            continueRecording();
        } else if (state == RECORDING) {
            stopRecording();
        }
    }
}


void initializeState() {
    /* Serial.println("On, and ready to go"); */
    state = STOPPED;
    ledRed();
}


void startRecording() {
    /* Serial.println("Recording started..."); */
    state = RECORDING;
    startTime = millis();
    aggregatePauseTime = 0;
    pauseStartTime = 0;
    ledGreen();
}


void pauseRecording() {
    Serial.println("Recording paused...");
    pauseStartTime = millis();
    state = PAUSED;
}


void continueRecording() {
    Serial.println("Recording continued...");
    state = RECORDING;
    uint32_t pauseEndTime = millis();
    aggregatePauseTime += pauseEndTime - pauseStartTime;
    ledGreen();
}


void cancelRecording() {
    Serial.println("Recording cancelled...");
    state = STOPPED;
    ledRed();
}


void stopRecording() {
    Serial.println("Recording stopped...");
    getElapsedTime();
    state = STOPPED;
    ledRed();
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
    if (state != RECORDING) {
        return;
    }

    elapsedMilliseconds = millis();
    int seconds = ((elapsedMilliseconds - startTime - aggregatePauseTime) / 1000) % 60;
    int minutes = ((elapsedMilliseconds - startTime - aggregatePauseTime) / 1000) / 60;
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

    if (state > INIT) {
        lcd.setCursor(7, 1);
        lcd.print(guesses);
    }

    if (state > STOPPED) {
        lcd.setCursor(0, 2);
        lcd.print(elapsedTime);
    }
}


void ledOff() {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void ledOn() {
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, HIGH);
}


void ledRed() {
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void ledBlue() {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, HIGH);
}


void ledGreen() {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void ledYellow() {
    analogWrite(LED_RED_PIN, HIGH);
    analogWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, LOW);
}
