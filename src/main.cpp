#include <Arduino.h>
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"

IMU imu(0.9);
Barometer baro;
LED leds;

void setup() {
  Serial.begin(115200);
  //imu.setupIMU(); SUCCESS
  leds.begin();
  //baro.begin(); SUCCESS
}

void loop() {
  //imu.update(); SUCCESS
  //imu.printValues(); SUCCESS
  leds.indicateStartup();
  //baro.update(); SUCCESS
  //baro.printAll(); SUCCESS

}
