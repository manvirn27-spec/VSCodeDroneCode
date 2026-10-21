#include "MotorMixer.h"
#include "driver/ledc.h"
#include <Arduino.h>

MotorMixer::MotorMixer()
{
}

void MotorMixer::begin(){
  ledcSetup(0, freq, resolution);
  ledcAttachPin(MOT1_PIN, 0);

  ledcSetup(1, freq, resolution);
  ledcAttachPin(MOT2_PIN, 1);

  ledcSetup(2, freq, resolution);
  ledcAttachPin(MOT3_PIN, 2);


  ledcSetup(3, freq, resolution);
  ledcAttachPin(MOT4_PIN, 3);

  

  calibrateMotors();
  stopMotors();
  delay(3000);
}

void MotorMixer::spinMotors(int throttle, int rollControl, int pitchControl, int yawControl)
{
  float MotorInput1;
  float MotorInput2;
  float MotorInput3;
  float MotorInput4;

  if (throttle > 1800)
    throttle = 1800;
  MotorInput1 = 1.024 * (throttle + rollControl - pitchControl - yawControl);
  MotorInput2 = 1.024 * (throttle + rollControl + pitchControl + yawControl);
  MotorInput3 = 1.024 * (throttle - rollControl - pitchControl + yawControl);
  MotorInput4 = 1.024 * (throttle - rollControl + pitchControl - yawControl);

  if (MotorInput1 > 2000)
    MotorInput1 = 1999;
  if (MotorInput2 > 2000)
    MotorInput2 = 1999;
  if (MotorInput3 > 2000)
    MotorInput3 = 1999;
  if (MotorInput4 > 2000)
    MotorInput4 = 1999;

  int ThrottleIdle = 1180;
  if (MotorInput1 < ThrottleIdle)
    MotorInput1 = ThrottleIdle;
  if (MotorInput2 < ThrottleIdle)
    MotorInput2 = ThrottleIdle;
  if (MotorInput3 < ThrottleIdle)
    MotorInput3 = ThrottleIdle;
  if (MotorInput4 < ThrottleIdle)
    MotorInput4 = ThrottleIdle;

  /*Debug Prints:*/
  Serial.printf("Mot1: %f, Mot2: %f, Mot3: %f, Mot 4: %f \n",
    MotorInput1, MotorInput2, MotorInput3, MotorInput4);
  uint32_t duty1 = (MotorInput1 / 20000.0) * ((1 << resolution) - 1);
  uint32_t duty2 = (MotorInput2 / 20000.0) * ((1 << resolution) - 1);
  uint32_t duty3 = (MotorInput3 / 20000.0) * ((1 << resolution) - 1);
  uint32_t duty4 = (MotorInput4 / 20000.0) * ((1 << resolution) - 1);
  
  ledcWrite(0, duty1);
  ledcWrite(1, duty2);
  ledcWrite(2, duty3);
  ledcWrite(3, duty4);
}

// Commands all motors to the cutoff value (used while disarmed).
void MotorMixer::stopMotors()
{
  uint32_t duty = (1000.f / 20000.0) * ((1 << resolution) - 1);
  
  ledcWrite(0, duty);
  ledcWrite(1, duty);
  ledcWrite(2, duty);
  ledcWrite(3, duty);
}

void MotorMixer::calibrateMotors()
{
  uint32_t duty = (2000.f / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(0, duty);
  ledcWrite(1, duty);
  ledcWrite(2, duty);
  ledcWrite(3, duty);
  delay(2000);
  duty = (1000.f / 20000.0) * ((1 << resolution) - 1);
  ledcWrite(0, duty);
  ledcWrite(1, duty);
  ledcWrite(2, duty);
  ledcWrite(3, duty);
}
