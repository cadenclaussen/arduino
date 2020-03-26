// BUTTON wiring:


const int GREEN_BUTTON_PIN = 5;
/* const int RED_BUTTON_PIN = 6; */
/* const int BLACK_BUTTON_PIN = 7; */


void setup() {
    Serial.begin(9600);
    pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);
    /* pinMode(RED_BUTTON_PIN, INPUT_PULLUP); */
    /* pinMode(BLACK_BUTTON_PIN, INPUT_PULLUP); */
}


void loop() {
    int greenButtonState = digitalRead(GREEN_BUTTON_PIN);
    if (greenButtonState == HIGH) {
        Serial.print("Green: HIGH  ");
    } else {
        Serial.print("Green: LOW  ");
    }

    /* int redButtonState = digitalRead(RED_BUTTON_PIN); */
    /* if (redButtonState == HIGH) { */
    /*     Serial.print("Red: HIGH  "); */
    /* } else { */
    /*     Serial.print("Red: LOW  "); */
    /* } */

    /* int blackButtonState = digitalRead(BLACK_BUTTON_PIN); */
    /* if (blackButtonState == HIGH) { */
    /*     Serial.print("Black: HIGH"); */
    /* } else { */
    /*     Serial.print("Black: LOW"); */
    /* } */

    Serial.println("");
    delay(100);
}
