#include "RTClib.h"


RTC_DS3231 rtc;


char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };


void setup () {

    Serial.begin(9600);

    rtc.begin();

    if (rtc.lostPower()) {
        // Use compilation date/time to reset
        Serial.println("RTC lost power.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    // Use an explicit date/time to reset
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}


void loop () {
    DateTime now = rtc.now();

    Serial.print("Time: ");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print("  ");
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print("  ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print("Temperature: ");
    int tempC = rtc.getTemperature();
    int tempF = (tempC * 1.8) + 32;
    Serial.print(tempF);
    Serial.println(" F");

    Serial.println();
    delay(3000);
}
