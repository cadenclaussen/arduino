// Common Cathode LED wiring:
// - Place common cathode LED into the breadboard in slots 1 (blue), 2 (green), 3 (ground), and 4 (red)
// - Wire Arduino PWM 2 to slot 1 (blue)
// - Wire Arduino PWM 3 to slot 2 (green)
// - Wire Arduino GND   to slot 3 (ground)
// - Wire Arduino PWM 4 to slot 4 (red)
// - Add 330 ohm resisters in slots 1, 2, and 4 between the wire from
//   the Arduino PWM and the Common Cathode's pins


const int BLUE = 2;
const int GREEN = 3;
const int RED = 4;


void setup() {
    Serial.begin(9600);
    pinMode(BLUE, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(RED, OUTPUT);
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
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
}


void on() {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
}


void red() {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
}


void blue() {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
}


void green() {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
}


void yellow() {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
}
