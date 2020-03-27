// Library: Cloned git clone https://github.com/Chris--A/Keypad.git to ~/Documents/Arduino/libraries
#include <Keypad.h>


const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {{ '1', '2', '3', 'A' },
                         { '4', '5', '6', 'B' },
                         { '7', '8', '9', 'C' },
                         { '*', '0', '#', 'D' }};
byte rowPins[ROWS] = { 38, 39, 40, 41 };
byte colPins[COLS] = { 42, 43, 44, 45 };


Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void setup(){
    Serial.begin(9600);
}


void loop(){
    char key = keypad.getKey();
    if (key) {
        Serial.println(key);
    }
}
