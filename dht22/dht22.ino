#include <DHT.h>
#include <math.h>


// NOTE: Requires the DHT and Adafruit Sensor libraries
//
// DHT22 wiring:
// - Insert DHT22 into breadboard in slots 1 (+), 2 (out), and 3 (-)
// - Wire Arduino 5V to slot 1 (+)
// - Wire Arduino PWM 5 to slot 2 (out)
// - Wire Arduino GND to slot 3 (-)


const int DHT22_PIN = 8;


DHT dht22(DHT22_PIN, DHT22);


int temperature;
int humidity;


void setup() {
    Serial.begin(9600);
    dht22.begin();
    delay(2000);
}


void loop() {
    getTemperatureHumidity();

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("F  Humidity: ");
    Serial.print(humidity);
    Serial.println();

    delay(2000);
}


void getTemperatureHumidity() {
    temperature = (int) round(dht22.readTemperature(true));
    humidity = (int) round(dht22.readHumidity());
}
