#include "LoRaLink.h"

SX1262 radio = new Module(PIN_LORA_NSS, PIN_LORA_DIO1, PIN_LORA_RESET, PIN_LORA_BUSY);

volatile bool packetReceived = false;
bool hasPreviousSequence = false;
uint16_t previousSequence = 0;
uint32_t lostPackets = 0;

void onPacketReceived() {
    packetReceived = true;
}

void setup() {
    Serial.begin(115200);

    int16_t state = beginLoRa(radio);
    if (state == RADIOLIB_ERR_NONE) {
        radio.setPacketReceivedAction(onPacketReceived);
        state = radio.startReceive();
    }
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print("LoRa init failed, code ");
        Serial.println(state);
        while (true) {
        }
    }
    Serial.println("LoRa receiver ready");
}

void loop() {
    if (!packetReceived) {
        return;
    }
    packetReceived = false;

    TelemetryPacket packet;
    size_t length = radio.getPacketLength();
    int16_t state = radio.readData(reinterpret_cast<uint8_t*>(&packet), sizeof(packet));
    if (state == RADIOLIB_ERR_CRC_MISMATCH) {
        Serial.println("Dropped packet: CRC error");
        return;
    }
    if (state != RADIOLIB_ERR_NONE) {
        Serial.print("Receive failed, code ");
        Serial.println(state);
        return;
    }
    if (length != sizeof(packet)) {
        Serial.print("Dropped packet: expected ");
        Serial.print(sizeof(packet));
        Serial.print(" bytes, got ");
        Serial.println(length);
        return;
    }

    if (hasPreviousSequence) {
        lostPackets += static_cast<uint16_t>(packet.sequence - previousSequence - 1);
    }
    hasPreviousSequence = true;
    previousSequence = packet.sequence;

    Serial.print("#");
    Serial.print(packet.sequence);
    Serial.print(" t=");
    Serial.print(packet.uptimeMs / 1000.0f, 1);
    Serial.print("s roll=");
    Serial.print(packet.rollCentideg / 100.0f, 2);
    Serial.print(" pitch=");
    Serial.print(packet.pitchCentideg / 100.0f, 2);
    Serial.print(" yaw=");
    Serial.print(packet.yawCentideg / 100.0f, 2);
    Serial.print(" alt=");
    Serial.print(packet.altitudeCm / 100.0f, 2);
    Serial.print("m batt=");
    Serial.print(packet.batteryMv / 1000.0f, 2);
    Serial.print("V | RSSI=");
    Serial.print(radio.getRSSI(), 1);
    Serial.print("dBm SNR=");
    Serial.print(radio.getSNR(), 1);
    Serial.print("dB lost=");
    Serial.println(lostPackets);
}
