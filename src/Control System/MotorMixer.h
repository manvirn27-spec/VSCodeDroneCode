#ifndef MotorMixer_H
#define MotorMixer_H
#include <ESP32Servo.h>

class MotorMixer{
  public:
    MotorMixer(int mot1, int mot2, int mot3, int mot4);
    
    void spinMotors(int throttle, int rollControl, int pitchControl, int yawControl);
    void stopMotors();
  private:
    Servo motors[4];
};

#endif
