#ifndef TELEMCOMMS_H
#define TELEMCOMMS_H

#include <Arduino.h>
#include <TelemetryFields.h>
#include <cmath>
#include <cstdint>
#include <limits>

// Sends telemetry to the DX-LR30 LoRa sender as I2C master on Wire1, so a fault
// on this bus cannot hang the IMU and barometer on Wire.
// Wiring: SDA_PIN -> DX-LR30 PB11, SCL_PIN -> DX-LR30 PB10, plus a shared ground.
// Both lines need external pull-ups to 3.3 V (4.7 kOhm); the internal ones are
// too weak for I2C_FREQ_HZ.
class TelemComms{
    public:
        void begin();
        // True once SEND_INTERVAL_MS has passed since the last send().
        bool isDue();
        void send(const TelemetryFields& fields);
    private:
        static constexpr int SDA_PIN = 6;
        static constexpr int SCL_PIN = 7;
        static constexpr uint32_t I2C_FREQ_HZ = 400000;
        // A frame takes under 1 ms at I2C_FREQ_HZ; this caps how long a stuck bus
        // can stall the control loop. The default is 50 ms. Counted in 1 ms
        // scheduler ticks, so the real limit can be up to 1 ms shorter.
        static constexpr uint16_t I2C_TIMEOUT_MS = 5;
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
