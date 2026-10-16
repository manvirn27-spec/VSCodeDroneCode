#include "LoRaLink.h"

constexpr uint32_t SEND_INTERVAL_MS = 100;

SX1262 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_RESET, PIN_LORA_BUSY);

TelemetryPacket packet = {};
uint32_t lastSendMs = 0;

void setup() {
    Serial.begin(115200);

    int16_t state = beginLoRa(radio);
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print("LoRa init failed, code ");
        Serial.println(state);
        while (true) {
        }
    }
    Serial.println("LoRa sender ready");
}

void loop() {
    if (millis() - lastSendMs < SEND_INTERVAL_MS) {
        return;
    }
    lastSendMs = millis();

    // Fake flight data so the receiver has changing values to display.
    float t = lastSendMs / 1000.0f;
    packet.uptimeMs = lastSendMs;
    packet.rollCentideg = 1500 * sinf(t);
    packet.pitchCentideg = 800 * cosf(t);
    packet.yawCentideg = (lastSendMs / 10) % 36000;
    packet.altitudeCm = 1000 + 500 * sinf(t / 4);
    packet.batteryMv = 12600 - (lastSendMs / 1000) % 1800;

    int16_t state = radio.transmit(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    if (state == RADIOLIB_ERR_NONE) {
        Serial.print("Sent #");
        Serial.println(packet.sequence);
    } else {
        Serial.print("Send failed, code ");
        Serial.println(state);
    }
    packet.sequence++;
}
