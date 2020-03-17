// LED wiring:
// - Control-2 to slot 1
// - Both ends of 330 ohm resister in slot 1 between Control-2 wire and LED
// - LED positive in slot 1 (long end of LED)
// - LED negative in slot 2 (short end of LED)
// - slot 2 to ground
//
// BUTTON wiring:
// - 5V to slot 10
// - 10K ohm resister from slot 10 to slot 11
// - slot 11 to slot 12
// - button in slot 12 (positive) and slot 14 (negative)
// - slot 14 to ground
// - slot 11 to Control-3 (should be on the outer portion of the circuit)

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
