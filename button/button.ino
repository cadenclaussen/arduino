const int LED = 2;
const int BUTTON = 3;


void setup() {
    pinMode(BUTTON, INPUT);
    pinMode(LED, OUTPUT);
}

void loop() {
    int buttonState = digitalRead(buttonPin);
}
