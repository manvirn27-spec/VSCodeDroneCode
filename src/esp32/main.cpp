#include <Arduino.h>
#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Peripherals/Receiver.h"
#include "Control System/FlightController.h" //Ultimately only include file

FlightController fc;
//Receiver elrs;


void setup() {
  Serial.begin(115200);
  //imu.setupIMU();
  //leds.begin();
  //baro.begin();
  fc.begin();
  //elrs.begin();

}

void loop() {
  //imu.update(); 
  //imu.printValues(); 
  //leds.indicateStartup();
  //baro.update(); 
  //baro.printAll(); 
  fc.execute();
  //elrs.update();
  //elrs.printAll();
}
