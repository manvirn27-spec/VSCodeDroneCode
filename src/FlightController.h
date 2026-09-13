#ifndef FlightController_H
#define FlightController_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "GPS.h"
#include "IMU.h"
#include "MotorMixer.h"
#include "PID.h"
#include "Receiver.h"

class FlightController{
  public:
    FlightController();

    void execute();
    void executeHoldPosition();
    void executeAutonomous();
    void executeRTH();
  private:
    Servo mot1;
    Servo mot2;
    Servo mot3;
    Servo mot4;
    
    IMU imu;
    GPS gps;
    MotorMixer motorMixer(mot1, mot2, mot3, mot4);
    PID rollAnglePID(1, 1, 1);
    PID pitchAnglePID(1, 1, 1);
    PID yawAnglePID(1, 1, 1);
    PID rollRatePID(1, 1, 1);
    PID pitchRatePID(1, 1, 1);
    PID yawRatePID(1, 1, 1);
    Receiver receiver;

    double requestedRollAngle;
    double requestedPitchAngle;
    double requestedYaw;
    double requestedThrottle;

    double currentRollRate;
    double currentPitchRate;
    double currentYawRate;
    
    double currentRollAngle;
    double currentPitchAngle;
    double currentYawAngle; //NOTE: Actually GPS heading but called "currentYawAngle" for consistency

    double currentX;
    double currentY;
    double currentZ;

    double currentVeloX;
    double currentVeloY;
    double currentVeloZ;


}