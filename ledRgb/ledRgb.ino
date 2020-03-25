// LED Common Cathode wiring:
// - Insert LED common cathode into the breadboard in slots 1 (blue), 2 (green), 3 (ground), and 4 (red)
// - Wire Arduino PWM 2 to slot 1 (blue)
// - Wire Arduino PWM 3 to slot 2 (green)
// - Wire Arduino GND   to slot 3 (ground)
// - Wire Arduino PWM 4 to slot 4 (red)
// - Add 220 ohm resisters in slots 1, 2, and 4 between the wire from
//   the Arduino PWM and the LED's pins


const int LED_BLUE_PIN = 2;
const int LED_GREEN_PIN = 3;
const int LED_RED_PIN = 4;


void setup() {
    Serial.begin(9600);
    pinMode(LED_BLUE_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
}


void loop() {
    /* off(); */
    /* delay(2000); */
    /* on(); */
    /* delay(2000); */
    /* blue(); */
    /* delay(2000); */
    /* green(); */
    /* delay(2000); */
    /* red(); */
    /* delay(2000); */
    /* yellow(); */
    /* delay(2000); */
    someColor();
    delay(2000);
    magenta();
    delay(2000);
    magenta2();
    delay(2000);
}


void off() {
    Serial.println("off");
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void on() {
    Serial.println("on");
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, HIGH);
}


void red() {
    Serial.println("red");
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void blue() {
    Serial.println("blue");
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, HIGH);
}


void green() {
    Serial.println("green");
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void yellow() {
    Serial.println("yellow");
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, HIGH);
    digitalWrite(LED_BLUE_PIN, LOW);
}


void someColor() {
    Serial.println("?");
    analogWrite(LED_RED_PIN, 0);
    analogWrite(LED_GREEN_PIN, 255);
    analogWrite(LED_BLUE_PIN, 255);
}


void magenta() {
    Serial.println("?");
    analogWrite(LED_RED_PIN, 255);
    analogWrite(LED_GREEN_PIN, 0);
    analogWrite(LED_BLUE_PIN, 255);
}


void magenta2() {
    Serial.println("?");
    analogWrite(LED_RED_PIN, 150);
    analogWrite(LED_GREEN_PIN, 0);
    analogWrite(LED_BLUE_PIN, 255);
}
