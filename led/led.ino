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
