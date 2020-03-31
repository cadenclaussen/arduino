#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>


// NOTE: Must download the Adafruit GPS Library!
//
// GPS wiring (power to ground):
// - 5V to Vin
// - GND to GND
// - RX to Control-5
// - TX to Control-6


const int RX = 5;
const int TX = 6;


SoftwareSerial softSerial(TX, RX);
Adafruit_GPS GPS(&softSerial);


String nmea1;
String nmea2;
char ch;


void setup() {
    Serial.begin(115200);
    GPS.begin(9600);
    GPS.sendCommand("$PGCMD,33,0*6D");
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    delay(1000);
}


void loop() {
    readGPS();
}


void readGPS() {
    clearGPS();
    while (!GPS.newNMEAreceived()) {
        ch = GPS.read();
    }
    GPS.parse(GPS.lastNMEA());
    nmea1 = GPS.lastNMEA();
    while (!GPS.newNMEAreceived()) {
        ch = GPS.read();
    }

    GPS.parse(GPS.lastNMEA());
    nmea2 = GPS.lastNMEA();

    Serial.println(nmea1);
    Serial.println(nmea2);
    Serial.println("");
}


void clearGPS() {
    Serial.println("Clearing GPS");
    while (!GPS.newNMEAreceived()) {
        ch = GPS.read();
    }
    GPS.parse(GPS.lastNMEA());
    while (!GPS.newNMEAreceived()) {
        ch = GPS.read();
    }

    GPS.parse(GPS.lastNMEA());
    Serial.println("DONE Clearing");
}
