// LED RGB wiring:
// - Insert LED common cathode into the breadboard in slots 1 (blue), 2 (green), 3 (ground), and 4 (red)
// - Wire Arduino PWM 2 to slot 1 (blue)
// - Wire Arduino PWM 3 to slot 2 (green)
// - Wire Arduino GND   to slot 3 (ground)
// - Wire Arduino PWM 4 to slot 4 (red)
// - Add 220 ohm resisters in slots 1, 2, and 4 between the wire from
//   the Arduino PWM and the LED's pins


const int BLUE_LED_PIN = 2;
const int GREEN_LED_PIN = 3;
const int RED_LED_PIN = 4;


void setup() {
    Serial.begin(9600);
    pinMode(BLUE_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
}


void loop() {
    off();
    delay(2000);
    on();
    delay(2000);
    blue();
    delay(2000);
    green();
    delay(2000);
    red();
    delay(2000);
    yellow();
    delay(2000);
    magenta();
    delay(2000);
    magenta2();
    delay(2000);
}


void off() {
    Serial.println("off");
    led(LOW, LOW, LOW);
}


void on() {
    Serial.println("on");
    led(HIGH, HIGH, HIGH);
}


void red() {
    Serial.println("red");
    led(HIGH, LOW, LOW);
}


void green() {
    Serial.println("green");
    led(LOW, HIGH, LOW);
}


void blue() {
    Serial.println("blue");
    led(LOW, LOW, HIGH);
}


void yellow() {
    Serial.println("yellow");
    led(HIGH, HIGH, LOW);
}


void magenta() {
    Serial.println("magenta");
    analogWrite(RED_LED_PIN, 255);
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, 255);
}


void magenta2() {
    Serial.println("magenta2");
    analogWrite(RED_LED_PIN, 150);
    analogWrite(GREEN_LED_PIN, 0);
    analogWrite(BLUE_LED_PIN, 255);
}


void led(int red, int green, int blue) {
    if (red == LOW || red == HIGH) {
        digitalWrite(RED_LED_PIN, red);
    } else {
        analogWrite(RED_LED_PIN, red);
    }

    if (green == LOW || green == HIGH) {
        digitalWrite(GREEN_LED_PIN, green);
    } else {
        analogWrite(GREEN_LED_PIN, green);
    }

    if (blue == LOW || blue == HIGH) {
        digitalWrite(BLUE_LED_PIN, blue);
    } else {
        analogWrite(BLUE_LED_PIN, blue);
    }
}
