// BUTTON wiring:
// - Insert button into breadboard in slots 3-5
// - Wire Arduino 5V to breadboard slot 1
// - Wire 10K ohm resister from breadboard slot 1 to breadboard slot 2
// - Wire slots 2 and 3 together
// - Wire Arduino GND to slot 5
// - Wire Arduino PWM 5 to breadboard slot 3 (in the outer portion of the circuit)


const int BUTTON_PIN = 5;
// const int BUTTON_PIN = 6;
// const int BUTTON_PIN = 7;


void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT);
}


void loop() {
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == HIGH) {
        Serial.println("HIGH");
    } else {
        Serial.println("LOW");
    }
    delay(100);
}
