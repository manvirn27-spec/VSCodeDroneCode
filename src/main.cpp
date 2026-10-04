#include <Arduino.h>
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"

IMU imu(0.9);
Barometer baro;

void setup() {
  Serial.begin(115200);
  imu.setupIMU(); 
}

void loop() {
  imu.update(); 
  imu.printValues();
}
