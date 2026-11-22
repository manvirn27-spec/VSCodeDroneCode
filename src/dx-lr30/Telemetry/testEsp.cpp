#include <Arduino.h>
#include <TelemetryFields.h>
#include <Wire.h>

// Stands in for the flight controller so the LoRa link can be tested on the bench.
// Wiring: SDA_PIN -> DX-LR30 PB11, SCL_PIN -> DX-LR30 PB10, with 4.7 kOhm
// pull-ups to 3.3 V on both lines.
#define POT_PIN 27
#define SDA_PIN 14
#define SCL_PIN 13
#define I2C_FREQ_HZ 400000

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN, I2C_FREQ_HZ);
}

void loop() {
  // Fake flight data so the signed fields carry changing, negative values.
  TelemetryFields fields = {};
  fields.uptimeMs = millis();
  float t = fields.uptimeMs / 1000.0f;
  fields.armed = (fields.uptimeMs / 10000) % 2;
  // The pot stands in for the throttle stick.
  fields.throttleUs = 1000 + analogReadMilliVolts(POT_PIN) * 1000 / 3300;
  fields.rollCentideg = 1500 * sinf(t);
  fields.pitchCentideg = 800 * cosf(t);
  fields.rollRateDecidps = 1500 * cosf(t);
  fields.pitchRateDecidps = -800 * sinf(t);
  fields.yawRateDecidps = 3000 * sinf(t / 2);
  fields.rollRateSetDecidps = 1600 * cosf(t);
  fields.pitchRateSetDecidps = -900 * sinf(t);
  fields.yawRateSetDecidps = 3200 * sinf(t / 2);
  fields.rollPidOutput = (fields.rollRateSetDecidps - fields.rollRateDecidps) * 0.45f / 10;
  fields.pitchPidOutput = (fields.pitchRateSetDecidps - fields.pitchRateDecidps) * 0.45f / 10;
  fields.yawPidOutput = (fields.yawRateSetDecidps - fields.yawRateDecidps) * 3 / 10;
  fields.altitudeCm = 1000 + 1500 * sinf(t / 4);

  const uint8_t* bytes = reinterpret_cast<const uint8_t*>(&fields);
  Wire.beginTransmission(TELEMETRY_I2C_ADDRESS);
  Wire.write(bytes, sizeof(fields));
  Wire.write(telemetryCrc(bytes, sizeof(fields)));
  uint8_t error = Wire.endTransmission();
  Serial.printf("t=%lu thr=%u i2c=%s\n", (unsigned long)fields.uptimeMs, fields.throttleUs,
                error == 0 ? "ok" : "failed");
  delay(50);
}
