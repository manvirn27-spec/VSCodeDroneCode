#include "TelemComms.h"

#include <Wire.h>

void TelemComms::begin(){
    Wire1.begin(SDA_PIN, SCL_PIN, I2C_FREQ_HZ);
    Wire1.setTimeOut(I2C_TIMEOUT_MS);
}

bool TelemComms::isDue(){
    return millis() - lastSendMs >= SEND_INTERVAL_MS;
}

void TelemComms::send(const TelemetryFields& fields){
    lastSendMs = millis();

    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&fields);
    // Blocks the control loop until the frame is clocked out, under 1 ms at I2C_FREQ_HZ.
    // A missing or busy LoRa module just drops this reading.
    Wire1.beginTransmission(TELEMETRY_I2C_ADDRESS);
    Wire1.write(bytes, sizeof(fields));
    Wire1.write(telemetryCrc(bytes, sizeof(fields)));
    Wire1.endTransmission();
}
