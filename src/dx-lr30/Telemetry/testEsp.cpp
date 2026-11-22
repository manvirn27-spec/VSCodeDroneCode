#include <Arduino.h>
#include <TelemetryFields.h>

// Stands in for the flight controller so the LoRa link can be tested on the bench.
#define POT_PIN 27
#define TX_PIN 14
// Must match ESP_BAUD in Sender.cpp.
#define TELEM_BAUD 115200

void setup() {
  Serial.begin(115200);
  Serial2.begin(TELEM_BAUD, SERIAL_8N1, -1, TX_PIN);
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

  char line[TELEMETRY_LINE_MAX + 1];
  if (formatTelemetryLine(line, sizeof(line), fields) > 0) {
    Serial2.println(line);
    Serial.println(line);
  }
  delay(50);
}
