#pragma once

#include <stddef.h>
#include <stdint.h>

// Telemetry from the flight controller. The ESP32 writes these fields to the
// DX-LR30 LoRa sender over I2C, and the sender forwards them over the air.
// Fields are declared only here, so the flight controller, sender and receiver
// always agree on the order and types.
// Columns: type, name, divisor to display units, decimals, label, unit.
#define TELEMETRY_FIELDS(X)                                               \
    X(uint32_t, uptimeMs,            1000, 1, "t",         "s")           \
    X(uint8_t,  armed,               1,    0, "armed",     "")            \
    X(uint16_t, throttleUs,          1,    0, "thr",       "us")          \
    X(int16_t,  rollCentideg,        100,  1, "roll",      "")            \
    X(int16_t,  pitchCentideg,       100,  1, "pitch",     "")            \
    X(int16_t,  rollRateDecidps,     10,   1, "rollRate",  "dps")         \
    X(int16_t,  pitchRateDecidps,    10,   1, "pitchRate", "dps")         \
    X(int16_t,  yawRateDecidps,      10,   1, "yawRate",   "dps")         \
    X(int16_t,  rollRateSetDecidps,  10,   1, "rollSet",   "dps")         \
    X(int16_t,  pitchRateSetDecidps, 10,   1, "pitchSet",  "dps")         \
    X(int16_t,  yawRateSetDecidps,   10,   1, "yawSet",    "dps")         \
    X(int16_t,  rollPidOutput,       1,    0, "rollOut",   "")            \
    X(int16_t,  pitchPidOutput,      1,    0, "pitchOut",  "")            \
    X(int16_t,  yawPidOutput,        1,    0, "yawOut",    "")            \
    X(int32_t,  altitudeCm,          100,  2, "alt",       "m")

// Sent over I2C as raw bytes, so both ends must agree on this exact layout.
// Both the ESP32 and the STM32 are little-endian.
struct __attribute__((packed)) TelemetryFields {
#define X(type, name, divisor, decimals, label, unit) type name;
    TELEMETRY_FIELDS(X)
#undef X
};

// 7-bit address the LoRa sender answers to on the ESP32's telemetry I2C bus.
constexpr uint8_t TELEMETRY_I2C_ADDRESS = 0x42;

// One I2C write: the TelemetryFields bytes followed by telemetryCrc() of them.
// I2C has no error checking of its own, and the bus runs past the motor wiring.
constexpr size_t TELEMETRY_FRAME_SIZE = sizeof(TelemetryFields) + 1;

// CRC-8, polynomial 0x07.
inline uint8_t telemetryCrc(const uint8_t* data, size_t length) {
    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int bit = 0; bit < 8; bit++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : crc << 1;
        }
    }
    return crc;
}
