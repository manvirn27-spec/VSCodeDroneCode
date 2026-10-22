#ifndef MotorMixer_H
#define MotorMixer_H
#include <ESP32Servo.h>
#include <Arduino.h>
#include "driver/ledc.h"

class MotorMixer{
  public:
    MotorMixer();
    
    void begin();
    
    void spinMotor(int amount);

    void spinMotors(int throttle, int rollControl, int pitchControl, int yawControl);
    void stopMotors();

    void calibrateMotors();
  private:
    const int MOT1_PIN = 39;
    const int MOT2_PIN = 40;
    const int MOT3_PIN = 41;
    const int MOT4_PIN = 42;

    const int freq = 50;       // 50Hz for standard ESCs/servos
    const int resolution = 14; // 14-bit resolution
};

#endif
