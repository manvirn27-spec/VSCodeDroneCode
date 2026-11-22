#include "TelemComms.h"

void TelemComms::begin(){
    // UART0 is free because Serial is USB CDC. RX is unused: the LoRa module never replies.
    Serial0.begin(BAUD, SERIAL_8N1, -1, TX_PIN);
}

bool TelemComms::isDue(){
    return millis() - lastSendMs >= SEND_INTERVAL_MS;
}

void TelemComms::send(const TelemetryFields& fields){
    lastSendMs = millis();

    char line[TELEMETRY_LINE_MAX + 1];
    size_t length = formatTelemetryLine(line, sizeof(line) - 1, fields);
    if (length == 0) {
        return;
    }
    line[length++] = '\n';
    // Does not stall the control loop: the line fits in the 128-byte UART FIFO,
    // and the previous one drained long ago (~10 ms per line at BAUD).
    Serial0.write(reinterpret_cast<const uint8_t*>(line), length);
}
