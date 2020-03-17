#include <DHT.h>


// NOTE: Must download both the DHT and Adafruit Sensor libraries!
//
// DHT22 wiring (power to ground):
// - 5V to slot 1
// - DHT22 in slot 1, 2, and 3
// - slot 3 to ground
// - slot 2 to Control-4


const int TEMPERATURE = 4;


DHT HT(TEMPERATURE, DHT22);


float humidity;
float temperature;


void setup() {
    Serial.begin(9600);
    HT.begin();
    delay(2000);
}


void loop() {
    humidity = HT.readHumidity();
    temperature = HT.readTemperature(true);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("F  Humidity: ");
    Serial.print(humidity);
    Serial.println();
    delay(2000);
}
