#include <Arduino.h>

#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Peripherals/Receiver.h"
#include "Control System/FlightController.h" //Ultimately only include file

FlightController fc;
// Receiver elrs;
//IMU imu{0.98};

void setup()
{
  Serial.begin(115200);
  //imu.begin();
  fc.begin();
  pinMode(16, OUTPUT);
}

void loop()
{
  // imu.update();
  // imu.printValues();
  // leds.indicateStartup();
  // baro.update();
  // baro.printAll();
  fc.executeRate();
  fc.calibrate(); // this should only run once once and then will not do anything once calibrated
  // elrs.update();
  // elrs.printAll();
}
