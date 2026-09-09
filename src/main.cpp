#include <Arduino.h>
#include "IMU.h"

IMU imu(0.98);

void setup() {
  Serial.begin(115200);
  imu.setupIMU();
  imu.calibrateIMU();
}

void loop() {
  imu.calculateValues();
  imu.printValues();
  delay(100);
}
