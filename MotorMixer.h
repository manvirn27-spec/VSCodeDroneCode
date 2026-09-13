#ifndef MotorMixer_H
#define MotorMixer_H
#include <ESP32Servo.h>

class MotorMixer{
  public:
    MotorMixer(Servo servos[]);
    void spinMotors(int throttle, int rollControl, int pitchControl, int yawControl);
  private:
    Servo motors[4];
};

#endif