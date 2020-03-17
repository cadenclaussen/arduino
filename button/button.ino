// LED wiring (power to ground):
// - POWER/CONTROL: Arduino 2 to breadboard column 1
// - Both ends of 330 ohm resister in breadboard column 1 between Arduino 2 connection and LED
// - LED positive in breadboard column 1 (long end of the LED)
// - LED negative in breadboard column 2 (short end of the LED)
// - GROUND: breadboard column 2 to Arduino GND
//
// BUTTON wiring (power to ground):
// - POWER: Arduino 5V to breadboard column 10
// - 10K ohm resister from breadboard column 10 to breadboard column 11
// - Wire from breadboard column 11 to breadboard column 12
// - button in breadboard column 12 (positive) and breadboard column 14 (negative)
// - GROUND: breadboard column 14 to Arduino GND
// - CONTROL: breadboard column 11 to Arduino 3 (should be on the outer portion of the circuit)


const int LED = 2;
const int BUTTON = 3;


void setup() {
    Serial.begin(9600);
    pinMode(BUTTON, INPUT);
    pinMode(LED, OUTPUT);
}


void loop() {
    int buttonState = digitalRead(BUTTON);
    Serial.println(buttonState);
    if (buttonState == HIGH) {
        digitalWrite(LED, LOW);
    } else {
        digitalWrite(LED, HIGH);
    }
    delay(1000);
}
