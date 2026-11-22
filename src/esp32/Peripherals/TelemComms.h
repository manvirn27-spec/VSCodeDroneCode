#ifndef TELEMCOMMS_H
#define TELEMCOMMS_H

#include <Arduino.h>
#include <TelemetryFields.h>
#include <cmath>
#include <cstdint>
#include <limits>

// Sends telemetry to the DX-LR30 LoRa sender over a transmit-only UART.
// Wiring: TX_PIN -> DX-LR30 PB11, plus a shared ground.
class TelemComms{
    public:
        void begin();
        // True once SEND_INTERVAL_MS has passed since the last send().
        bool isDue();
        void send(const TelemetryFields& fields);
    private:
        // Must match ESP_BAUD in the LoRa sender.
        static constexpr uint32_t BAUD = 115200;
        static constexpr int TX_PIN = 6;
        // Twice the LoRa packet rate, so each packet carries a reading under 50 ms old.
        static constexpr uint32_t SEND_INTERVAL_MS = 50;

        uint32_t lastSendMs = 0;
};

// Converts `value` to a fixed-point integer `value * scale`, rounded and clamped to T.
template <typename T>
T toFixedPoint(double value, double scale){
    double scaled = value * scale;
    if (std::isnan(scaled)) {
        return 0;
    }
    if (scaled <= std::numeric_limits<T>::min()) {
        return std::numeric_limits<T>::min();
    }
    if (scaled >= std::numeric_limits<T>::max()) {
        return std::numeric_limits<T>::max();
    }
    return static_cast<T>(std::lround(scaled));
}

#endif
