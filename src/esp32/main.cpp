#include <Arduino.h>

#include "driver/ledc.h"
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Peripherals/Receiver.h"
#include "Control System/MotorMixer.h"
#include "Control System/FlightController.h" //Ultimately only include file

FlightController fc;

void setup(){
  fc.begin();
}

void loop(){
  fc.executeRate();
  fc.calibrate();
}






















/*
const int freq = 50;       // 50Hz for standard ESCs/servos
const int resolution = 14; // 14-bit resolution

void setup() {
  delay(1000);
  ledcSetup(0, freq, resolution);
  ledcAttachPin(39, 0);

  ledcSetup(1, freq, resolution);
  ledcAttachPin(40, 1);

  ledcSetup(2, freq, resolution);
  ledcAttachPin(41, 2);


  ledcSetup(3, freq, resolution);
  ledcAttachPin(42, 3);

  uint32_t duty1 = (1000 / 20000.0) * ((1 << resolution) - 1);
  
  ledcWrite(0, duty1);
  ledcWrite(1, duty1);
  ledcWrite(2, duty1);
  ledcWrite(3, duty1);

  delay(3000);
}

void loop() {
  uint32_t duty1 = (1500 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(0, duty1);
  duty1 = (1000 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(3, duty1);

  delay(3000);

  duty1 = (1000 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(0, duty1);
  duty1 = (1500 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(1, duty1);

  delay(3000);

  duty1 = (1000 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(1, duty1);
  duty1 = (1500 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(2, duty1);

  delay(3000);

  duty1 = (1000 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(2, duty1);
  duty1 = (1500 / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(3, duty1);

  delay(3000);
}
*/