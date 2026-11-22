#include <Wire.h>

#include "LoRaLink.h"

//TODO: implement something similar to CUBIC TCP to determine max bandwith at any given time
constexpr uint32_t SEND_INTERVAL_MS = 100;

// The ESP32 is the I2C master and writes one telemetry frame per reading
// (see TELEMETRY_FRAME_SIZE); we are a slave at TELEMETRY_I2C_ADDRESS on I2C2.
constexpr uint32_t PIN_ESP_SDA = PB11;
constexpr uint32_t PIN_ESP_SCL = PB10;
constexpr uint32_t ESP_TIMEOUT_MS = 500;

SX1262 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_RESET, PIN_LORA_BUSY);
TwoWire EspWire(PIN_ESP_SDA, PIN_ESP_SCL);

TelemetryPacket packet = {};
uint32_t lastSendMs = 0;
uint32_t lastEspDataMs = 0;
bool hasEspData = false;
// Starts true so the first missing reading is reported at boot.
bool espDataFresh = true;

// Written by onEspReceive() in interrupt context; only read with interrupts off.
volatile uint8_t espFrame[TELEMETRY_FRAME_SIZE];
volatile bool espFrameReady = false;

// Runs in interrupt context once the ESP32 ends a write. Keeps only the newest
// frame, since the ESP32 sends faster than we transmit.
void onEspReceive(int byteCount) {
    if (byteCount == static_cast<int>(TELEMETRY_FRAME_SIZE)) {
        for (size_t i = 0; i < TELEMETRY_FRAME_SIZE; i++) {
            espFrame[i] = EspWire.read();
        }
        espFrameReady = true;
    }
    // Wire ignores every later write until this one is fully read.
    while (EspWire.available()) {
        EspWire.read();
    }
}

// Only touches the packet once the frame passes its CRC, so a corrupted frame
// can never leave it half updated.
void applyEspFrame() {
    uint8_t frame[TELEMETRY_FRAME_SIZE];
    noInterrupts();
    if (!espFrameReady) {
        interrupts();
        return;
    }
    for (size_t i = 0; i < TELEMETRY_FRAME_SIZE; i++) {
        frame[i] = espFrame[i];
    }
    espFrameReady = false;
    interrupts();

    if (telemetryCrc(frame, sizeof(TelemetryFields)) != frame[sizeof(TelemetryFields)]) {
        return;
    }
    TelemetryFields fields;
    memcpy(&fields, frame, sizeof(fields));
#define X(type, name, divisor, decimals, label, unit) packet.name = fields.name;
    TELEMETRY_FIELDS(X)
#undef X
    lastEspDataMs = millis();
    hasEspData = true;
}

void setup() {
    Serial.begin(115200);
    EspWire.begin(TELEMETRY_I2C_ADDRESS, false, false);
    EspWire.onReceive(onEspReceive);

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
    applyEspFrame();

    if (millis() - lastSendMs < SEND_INTERVAL_MS) {
        return;
    }
    lastSendMs = millis();

    // Stale readings are not sent, so the receiver never shows old data as live.
    bool fresh = hasEspData && millis() - lastEspDataMs < ESP_TIMEOUT_MS;
    if (fresh != espDataFresh) {
        espDataFresh = fresh;
        Serial.println(fresh ? "ESP32 data received, transmitting"
                             : "No data from ESP32, transmit paused");
    }
    if (!fresh) {
        return;
    }

    int16_t state = radio.transmit(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print("Transmit failed, code ");
        Serial.println(state);
    }
    packet.sequence++;
}
