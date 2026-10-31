#include <Arduino.h>

#define POT_PIN 27
#define TX_PIN 14

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, -1, TX_PIN);
}

void loop() {
  uint32_t potMv = analogReadMilliVolts(POT_PIN);

  // Fake flight data so the signed fields carry changing, negative values.
  unsigned long uptimeMs = millis();
  float t = uptimeMs / 1000.0f;
  int roll = 1500 * sinf(t);
  int pitch = 800 * cosf(t);
  int yaw = (uptimeMs / 10) % 36000 - 18000;
  long altitudeCm = 1000 + 1500 * sinf(t / 4);

  // Must match the field order the sender expects:
  // uptimeMs,rollCentideg,pitchCentideg,yawCentideg,altitudeCm,batteryMv
  char line[64];
  snprintf(line, sizeof(line), "%lu,%d,%d,%d,%ld,%lu", uptimeMs, roll, pitch, yaw,
           altitudeCm, (unsigned long)potMv);
  Serial2.println(line);
  Serial.println(line);
  delay(50);
}
