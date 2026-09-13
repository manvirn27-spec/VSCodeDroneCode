#include "MotorMixer.h"
#include <ESP32Servo.h>

MotorMixer::MotorMixer(Servo servo[]){
  for(int i = 0; i < 4; i++){
    motors[i] = servo[i];
  }
}

void MotorMixer::spinMotors(int throttle, int rollControl, int pitchControl, int yawControl){
  double MotorInput1;
  double MotorInput2;
  double MotorInput3;
  double MotorInput4;

  if (throttle > 1800) throttle = 1800;
  MotorInput1= 1.024*(throttle+rollControl-pitchControl-yawControl);
  MotorInput2= 1.024*(throttle+rollControl+pitchControl+yawControl);
  MotorInput3= 1.024*(throttle-rollControl-pitchControl+yawControl);
  MotorInput4= 1.024*(throttle-rollControl+pitchControl-yawControl);
  if (MotorInput1 > 2000)MotorInput1 = 1999;
  if (MotorInput2 > 2000)MotorInput2 = 1999; 
  if (MotorInput3 > 2000)MotorInput3 = 1999; 
  if (MotorInput4 > 2000)MotorInput4 = 1999;
  int ThrottleIdle=1180;
  if (MotorInput1 < ThrottleIdle) MotorInput1 =  ThrottleIdle;
  if (MotorInput2 < ThrottleIdle) MotorInput2 =  ThrottleIdle;
  if (MotorInput3 < ThrottleIdle) MotorInput3 =  ThrottleIdle;
  if (MotorInput4 < ThrottleIdle) MotorInput4 =  ThrottleIdle;
  int ThrottleCutOff=1000;
  if (throttle<1050) {
    MotorInput1=ThrottleCutOff; 
    MotorInput2=ThrottleCutOff;
    MotorInput3=ThrottleCutOff; 
    MotorInput4=ThrottleCutOff;
  }
  motors[0].writeMicroseconds(MotorInput1);
  motors[1].writeMicroseconds(MotorInput2);
  motors[2].writeMicroseconds(MotorInput3); 
  motors[3].writeMicroseconds(MotorInput4);
}
