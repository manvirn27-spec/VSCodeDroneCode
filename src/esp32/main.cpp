#include <Arduino.h>
#include <ESP32Servo.h>

#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Peripherals/Receiver.h"
#include "Control System/MotorMixer.h"
#include "Control System/FlightController.h" //Ultimately only include file

FlightController fc;

void setup() {
  Serial.begin(115200);
  fc.begin();
}

void loop() {
  fc.executeRate();
  fc.calibrate();
}