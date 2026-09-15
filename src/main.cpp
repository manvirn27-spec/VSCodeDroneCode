#include <Arduino.h>
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Control System/FlightController.h" //Ultimately only include file

IMU imu(0.9);
Barometer baro;
LED leds;
FlightController fc;


void setup() {
  Serial.begin(115200);
  //imu.setupIMU();
  //leds.begin();
  //baro.begin();
  fc.begin();

}

void loop() {
  //imu.update(); 
  //imu.printValues(); 
  //leds.indicateStartup();
  //baro.update(); 
  //baro.printAll(); 
  fc.execute();
}
