// Common Cathode LED wiring:
// - Insert common cathode LED into the breadboard in slots 1 (blue), 2 (green), 3 (ground), and 4 (red)
// - Wire Arduino PWM 2 to slot 1 (blue)
// - Wire Arduino PWM 3 to slot 2 (green)
// - Wire Arduino GND   to slot 3 (ground)
// - Wire Arduino PWM 4 to slot 4 (red)
// - Add 330 ohm resisters in slots 1, 2, and 4 between the wire from
//   the Arduino PWM and the Common Cathode's pins


const int LED_BLUE = 2;
const int LED_GREEN = 3;
const int LED_RED = 4;


void setup() {
    Serial.begin(9600);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
}


void loop() {
    on();
    delay(1000);
    blue();
    delay(1000);
    green();
    delay(1000);
    red();
    delay(1000);
    yellow();
    delay(1000);
    off();
    delay(1000);
}


void off() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
}


void on() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, HIGH);
}


void red() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
}


void blue() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
}


void green() {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
}


void yellow() {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
}
