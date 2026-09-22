#include <Arduino.h>

constexpr uint32_t LED_PIN = PC13;

// void setup() {
//     Serial.begin(115200);
//     pinMode(LED_PIN, OUTPUT);
// }

// void loop() {
//     digitalWrite(LED_PIN, LOW);
//     delay(500);

//     digitalWrite(LED_PIN, HIGH);
//     delay(500);
// }

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_PIN, LOW);
    delay(500);

    digitalWrite(LED_PIN, HIGH);
    delay(500);
}