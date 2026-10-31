#include <limits>

#include "LoRaLink.h"

//TODO: implement something similar to CUBIC TCP to determine max bandwith at any given time
constexpr uint32_t SEND_INTERVAL_MS = 100;

// The ESP32 sends one line per reading: the TelemetryPacket fields in order,
// comma separated, minus sequence, which we own so the receiver can count
// lost packets:
//   uptimeMs,rollCentideg,pitchCentideg,yawCentideg,altitudeCm,batteryMv
// It must use USART3: PA10 (USART1 RX) is also driven by the board's CH340C.
constexpr uint32_t PIN_ESP_RX = PB11;
constexpr uint32_t PIN_ESP_TX = PB10;
constexpr uint32_t ESP_BAUD = 9600;
constexpr uint32_t ESP_TIMEOUT_MS = 500;
// The longest valid line is 49 characters.
constexpr size_t ESP_LINE_MAX = 64;

SX1262 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_RESET, PIN_LORA_BUSY);
Uart EspSerial(PIN_ESP_RX, PIN_ESP_TX);

TelemetryPacket packet = {};
uint32_t lastSendMs = 0;
uint32_t lastEspLineMs = 0;
char espLine[ESP_LINE_MAX + 1];
size_t espLineLength = 0;
// Starts true so a line already in progress when we boot is thrown away.
bool discardEspLine = true;

// Parses one integer that must end at `separator` and fit in T, then steps
// the cursor past the separator.
template <typename T>
bool parseField(const char*& cursor, char separator, T& value) {
    char* end;
    long long parsed = strtoll(cursor, &end, 10);
    if (end == cursor || *end != separator || parsed < std::numeric_limits<T>::min() ||
        parsed > std::numeric_limits<T>::max()) {
        return false;
    }
    value = parsed;
    cursor = end + 1;
    return true;
}

// Only touches the packet once every field is valid, so a corrupted line can
// never leave it half updated.
bool applyEspLine(const char* line) {
    uint32_t uptimeMs;
    int16_t rollCentideg;
    int16_t pitchCentideg;
    int16_t yawCentideg;
    int32_t altitudeCm;
    uint16_t batteryMv;

    const char* cursor = line;
    if (!parseField(cursor, ',', uptimeMs) || !parseField(cursor, ',', rollCentideg) ||
        !parseField(cursor, ',', pitchCentideg) || !parseField(cursor, ',', yawCentideg) ||
        !parseField(cursor, ',', altitudeCm) || !parseField(cursor, '\0', batteryMv)) {
        return false;
    }

    packet.uptimeMs = uptimeMs;
    packet.rollCentideg = rollCentideg;
    packet.pitchCentideg = pitchCentideg;
    packet.yawCentideg = yawCentideg;
    packet.altitudeCm = altitudeCm;
    packet.batteryMv = batteryMv;
    return true;
}

// Drains everything the ESP32 has sent so the packet always carries the newest
// reading; the ESP32 sends faster than we transmit, so a backlog would build up.
void readEspLines() {
    while (EspSerial.available()) {
        char c = EspSerial.read();
        if (c == '\n') {
            espLine[espLineLength] = '\0';
            if (!discardEspLine && applyEspLine(espLine)) {
                lastEspLineMs = millis();
            }
            espLineLength = 0;
            discardEspLine = false;
        } else if (c == '\r' || discardEspLine) {
            continue;
        } else if (espLineLength < ESP_LINE_MAX) {
            espLine[espLineLength++] = c;
        } else {
            discardEspLine = true;
        }
    }
}

void setup() {
    Serial.begin(115200);
    EspSerial.begin(ESP_BAUD);

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
    readEspLines();

    if (millis() - lastSendMs < SEND_INTERVAL_MS) {
        return;
    }
    lastSendMs = millis();

    int16_t state = radio.transmit(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    packet.sequence++;
}
