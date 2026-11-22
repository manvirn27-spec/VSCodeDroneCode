#include <limits>

#include "LoRaLink.h"

//TODO: implement something similar to CUBIC TCP to determine max bandwith at any given time
constexpr uint32_t SEND_INTERVAL_MS = 100;

// The ESP32 sends one line per reading: the TELEMETRY_FIELDS values in order,
// comma separated, newline terminated.
// It must use USART3: PA10 (USART1 RX) is also driven by the board's CH340C.
constexpr uint32_t PIN_ESP_RX = PB11;
constexpr uint32_t PIN_ESP_TX = PB10;
constexpr uint32_t ESP_BAUD = 115200;
constexpr uint32_t ESP_TIMEOUT_MS = 500;
constexpr size_t ESP_LINE_MAX = TELEMETRY_LINE_MAX;

SX1262 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_RESET, PIN_LORA_BUSY);
Uart EspSerial(PIN_ESP_RX, PIN_ESP_TX);

TelemetryPacket packet = {};
uint32_t lastSendMs = 0;
uint32_t lastEspLineMs = 0;
bool hasEspData = false;
// Starts true so the first missing reading is reported at boot.
bool espDataFresh = true;
char espLine[ESP_LINE_MAX + 1];
size_t espLineLength = 0;
// Starts true so a line already in progress when we boot is thrown away.
bool discardEspLine = true;

// Parses one integer that must end at a comma or the end of the line and fit
// in T, then steps the cursor past the comma.
template <typename T>
bool parseField(const char*& cursor, T& value) {
    char* end;
    long long parsed = strtoll(cursor, &end, 10);
    if (end == cursor || (*end != ',' && *end != '\0') ||
        parsed < std::numeric_limits<T>::min() || parsed > std::numeric_limits<T>::max()) {
        return false;
    }
    value = parsed;
    cursor = *end == ',' ? end + 1 : end;
    return true;
}

// Only touches the packet once every field is valid, so a corrupted line can
// never leave it half updated.
bool applyEspLine(const char* line) {
    TelemetryPacket parsed = packet;
    const char* cursor = line;
    // Packed members cannot bind to a reference, so each goes through a local.
#define X(type, name, divisor, decimals, label, unit) \
    {                                                 \
        type value;                                   \
        if (!parseField(cursor, value)) {             \
            return false;                             \
        }                                             \
        parsed.name = value;                          \
    }
    TELEMETRY_FIELDS(X)
#undef X
    // Rejects extra fields and a trailing comma.
    if (*cursor != '\0' || cursor[-1] == ',') {
        return false;
    }

    packet = parsed;
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
                hasEspData = true;
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

    // Stale readings are not sent, so the receiver never shows old data as live.
    bool fresh = hasEspData && millis() - lastEspLineMs < ESP_TIMEOUT_MS;
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
