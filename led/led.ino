// LED wiring (power to ground):
// - Control-2 to slot 1
// - Both ends of 330 ohm resister in slot 1 between Control-2 wire and LED
// - LED positive in slot 1 (long end of the LED)
// - LED negative in slot 2 (short end of the LED)
// - slot 2 to ground
//
const int LED = 2;


void setup() {
    pinMode(LED, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED, HIGH);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
}
