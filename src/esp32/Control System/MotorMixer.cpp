#include "MotorMixer.h"
#include <ESP32Servo.h>

MotorMixer::MotorMixer(int mot1, int mot2, int mot3, int mot4) {
    motors[0].attach(mot1);
    motors[1].attach(mot2);
    motors[2].attach(mot3);
    motors[3].attach(mot4);
}

void MotorMixer::spinMotors(int throttle, int rollControl, int pitchControl, int yawControl){
  float MotorInput1;
  float MotorInput2;
  float MotorInput3;
  float MotorInput4;

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

/*Debug Prints:*/
  //Serial.printf("Mot1: %f, Mot2: %f, Mot3: %f, Mot 4: %f \n", 
  //  MotorInput1, MotorInput2, MotorInput3, MotorInput4);

  motors[0].writeMicroseconds(MotorInput1);
  motors[1].writeMicroseconds(MotorInput2);
  motors[2].writeMicroseconds(MotorInput3); 
  motors[3].writeMicroseconds(MotorInput4);
}

// Commands all motors to the cutoff value (used while disarmed).
void MotorMixer::stopMotors(){
  for(int i = 0; i < 4; i++){
    motors[i].writeMicroseconds(1000);
  }
}

void MotorMixer::calibrateMotors(){
  for(int i = 0; i < 4; i++){
    motors[i].writeMicroseconds(2000);
  }
  delay(2000);
  for(int i = 0; i < 4; i++){
    motors[i].writeMicroseconds(1000);
  }
}

