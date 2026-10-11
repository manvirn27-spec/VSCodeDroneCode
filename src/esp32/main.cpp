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
  fc.executeRate();
  //imu.update();
  //Serial.println(imu.getRollRate());
}
