#include <Arduino.h>

#include "driver/ledc.h"
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Peripherals/Receiver.h"
#include "Peripherals/GPS.h"

#include "Control System/FlightController.h"

IMU imu{0.501};
Barometer baro;
Receiver receiver;
FlightController fc;
LED leds;
GPS gps;

int16_t compassX;
int16_t compassY;
int16_t compassZ;


void setup(){
  Serial.begin(115200);
  fc.begin();

  imu.begin();
  baro.begin();
  receiver.begin();
  gps.begin();

  leds.begin();
  leds.indicateStartup();

  delay(3000);
}

void loop(){
  imu.update();
  receiver.update();
  gps.readCompass(compassX, compassY, compassZ);
  gps.update();

  fc.executeRate(imu, receiver, leds);
}