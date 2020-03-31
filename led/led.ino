// LED wiring:
// - Insert LED into breadboard in slots 1 (+ long end) and 2 (- short end)
// - Wire Arduino PWM 2 to slot 1
// - Insert a 330 ohm resistor into slot 1 between the Arduino control wire and the LED +
// - Wire Arduino GND to slot 2


const int LED_PIN = 2;


void setup() {
    pinMode(LED_PIN, OUTPUT);
}


void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
}
