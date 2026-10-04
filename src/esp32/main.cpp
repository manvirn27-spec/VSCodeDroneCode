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
  fc.begin();
}

void loop()
{
  fc.executeRate();
}
