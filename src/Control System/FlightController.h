#ifndef FlightController_H
#define FlightController_H

#include <Arduino.h>
#include <ESP32Servo.h>

#include "MotorMixer.h"
#include "PID.h"

#include "Peripherals/GPS.h"
#include "Peripherals/IMU.h"
#include "Peripherals/Receiver.h"
#include "Peripherals/Barometer.h"
#include "Peripherals/LED.h"

class FlightController{
  public:
    void begin();

    void execute();
    void executeHoldPosition();
    void executeAutonomous();
    void executeRTH();

    bool isArmed();
  private:
    // Arm switch configuration (CRSF channel values are ~1000-2000us)
    static const int ARM_SWITCH_THRESHOLD = 1500; // aux above this = switch ON
    static const int ARM_THROTTLE_MAX = 1050;     // throttle must be below this to arm

    void updateArmState();
    void arm();
    int disarm();

    bool armed = false;
    bool armSwitchWasOn = false; // previous switch position, used to detect OFF->ON edges

    Servo motors[4];

    IMU imu{0.98};
    GPS gps;
    Barometer baro;
    Receiver receiver;
    LED leds;


    MotorMixer motorMixer{39, 40, 41, 42};
    PID rollAnglePID{1, 1, 1};
    PID pitchAnglePID{1, 1, 1};
    PID yawAnglePID{1, 1, 1};
    PID rollRatePID{1, 1, 1};
    PID pitchRatePID{1, 1, 1};
    PID yawRatePID{1, 1, 1};

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
};

#endif