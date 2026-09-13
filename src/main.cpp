#include <Arduino.h>
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"

IMU imu(0.9);
Barometer baro;

void setup() {
  Serial.begin(115200);
  //imu.setupIMU(); SUCCESS
  baro.begin();
}

void loop() {
  //imu.update(); SUCCESS
  //imu.printValues(); SUCCESS
  baro.update();
  baro.printAll();
  delay(1000);

}
