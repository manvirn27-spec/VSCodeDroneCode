#include <Arduino.h>

#include "Peripherals/IMU.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"
#include "Peripherals/Receiver.h"
#include "Control System/FlightController.h" //Ultimately only include file

/*TODO
TIM(make new class):
  Telemetry integration
  (Battery Voltage, requested roll rate, requested pitch rate, roll rate error, pitch rate error)

Aiken(Flight controller execute() method and LED class):
  Calibrate motors at press of controller button. Finish LED class

Manvir:
  Integrate GPS. Setup interrupts for barometer. Work on attitude and altitude estimation

Arjun:
  GPS print all method.
*/

FlightController fc;
// Receiver elrs;

void setup()
{
  Serial.begin(115200);
  // imu.setupIMU();
  // leds.begin();
  // baro.begin();
  fc.begin();
  // elrs.begin();
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
