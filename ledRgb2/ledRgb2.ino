int bluePin = 2;
int greenPin = 3;
int redPin = 4;


void setup() {
    Serial.begin(9600);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
}


void loop() {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(3000);

    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
    delay(3000);

    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
    delay(3000);

    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
    delay(3000);

    analogWrite(redPin, 255);
    analogWrite(greenPin, 100);
    analogWrite(bluePin, 0);
    delay(3000);

    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    delay(3000);

    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 100);
    delay(3000);

    digitalWrite(redPin, LOW);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 80);
    delay(3000);
}
