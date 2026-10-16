#pragma once

#include <Arduino.h>
#include <RadioLib.h>

// DX-LR30 dev board wiring between the STM32F103 and the SX1262.
// SPI1 uses the default pins: PA5 = SCK, PA6 = MISO, PA7 = MOSI.
constexpr uint32_t PIN_LORA_NSS = PA4;
constexpr uint32_t PIN_LORA_DIO1 = PC15;
constexpr uint32_t PIN_LORA_RESET = PA3;
constexpr uint32_t PIN_LORA_BUSY = PA2;
constexpr uint32_t PIN_LORA_TXEN = PA0;
constexpr uint32_t PIN_LORA_RXEN = PA1;

// Both modules must use identical settings or they will not hear each other.
constexpr float LORA_FREQ_MHZ = 915.0;
constexpr float LORA_BANDWIDTH_KHZ = 250.0;
constexpr uint8_t LORA_SPREADING_FACTOR = 7;
constexpr uint8_t LORA_CODING_RATE = 5; // 4/5
constexpr uint8_t LORA_SYNC_WORD = 0x12;
constexpr int8_t LORA_TX_POWER_DBM = 22;
constexpr uint16_t LORA_PREAMBLE_LENGTH = 8;
// The module clocks the SX1262 from a crystal, not a DIO3-powered TCXO.
constexpr float LORA_TCXO_VOLTAGE = 0.0;

// Sent over the air as raw bytes, so both ends must agree on this exact layout.
struct __attribute__((packed)) TelemetryPacket {
    uint16_t sequence;
    uint32_t uptimeMs;
    int16_t rollCentideg;
    int16_t pitchCentideg;
    int16_t yawCentideg;
    int32_t altitudeCm;
    uint16_t batteryMv;
};

inline int16_t beginLoRa(SX1262& radio) {
    int16_t state = radio.begin(LORA_FREQ_MHZ, LORA_BANDWIDTH_KHZ, LORA_SPREADING_FACTOR,
                                LORA_CODING_RATE, LORA_SYNC_WORD, LORA_TX_POWER_DBM,
                                LORA_PREAMBLE_LENGTH, LORA_TCXO_VOLTAGE);
    if (state != RADIOLIB_ERR_NONE) {
        return state;
    }

    // The antenna switch is driven by the STM32, not by the SX1262's DIO2.
    radio.setRfSwitchPins(PIN_LORA_RXEN, PIN_LORA_TXEN);
    return radio.setCRC(2);
}
