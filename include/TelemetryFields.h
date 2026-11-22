#pragma once

#include <stdint.h>
#include <stdio.h>

// Telemetry from the flight controller. The ESP32 sends these fields to the
// DX-LR30 LoRa sender as one comma-separated line, in this order, and the sender
// forwards them over the air. Fields are declared only here, so the flight
// controller, sender and receiver always agree on the order and types.
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

struct TelemetryFields {
#define X(type, name, divisor, decimals, label, unit) type name;
    TELEMETRY_FIELDS(X)
#undef X
};

// Every field at its widest is 109 characters.
constexpr size_t TELEMETRY_LINE_MAX = 128;

// Writes `fields` as one comma-separated line, without a line ending.
// Returns the line length, or 0 if it does not fit in `size`.
inline size_t formatTelemetryLine(char* line, size_t size, const TelemetryFields& fields) {
    size_t length = 0;
    const char* separator = "";
#define X(type, name, divisor, decimals, label, unit)                                \
    {                                                                                \
        int written = snprintf(line + length, size - length, "%s%lld", separator,  \
                               static_cast<long long>(fields.name));               \
        if (written < 0 || static_cast<size_t>(written) >= size - length) {          \
            return 0;                                                                \
        }                                                                            \
        length += written;                                                           \
        separator = ",";                                                             \
    }
    TELEMETRY_FIELDS(X)
#undef X
    return length;
}
