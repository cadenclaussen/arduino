#include <RTClib.h>


RTC_DS3231 ds3231;


char dateTime[19];
int temperature;


void setup() {
    Serial.begin(9600);
    delay(2000);

    ds3231.begin();

    if (ds3231.lostPower()) {
        // Use compilation date/time to reset
        Serial.println("RTC lost power.");
        ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Use an explicit date/time to reset
    // ds3231.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
}


void loop() {
    getDateTime();
    getTemperature();

    Serial.println(dateTime);
    Serial.println(temperature);

    delay(2000);
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


void getTemperature() {
    temperature = (ds3231.getTemperature() * 1.8) + 32;
}
