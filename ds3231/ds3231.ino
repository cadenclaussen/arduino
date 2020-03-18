#include "RTClib.h"


RTC_DS3231 ds3231;


String date;
String time;
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

    Serial.println(date);
    Serial.println(time);
    Serial.println(temperature);

    delay(2000);
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
    temperature = (ds3231.getTemperature() * 1.8) + 32;
}
