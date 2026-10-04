#ifndef FlightController_H
#define FlightController_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <cstdint>

#include "MotorMixer.h"
#include "PID.h"

#include "../Peripherals/GPS.h"
#include "../Peripherals/IMU.h"
#include "../Peripherals/Receiver.h"
#include "../Peripherals/Barometer.h"
#include "../Peripherals/LED.h"

class FlightController
{
public:
  void begin();
  void calibrate();
  void executeAngle();
  void executeRate();
  void executeHoldPosition();
  void executeAutonomous();
  void executeRTH();

  bool isArmed();

private:
  // Arm switch configuration (CRSF channel values are ~1000-2000us)
  static const int ARM_SWITCH_THRESHOLD = 0; // aux above this = switch ON
  static const int ARM_THROTTLE_MAX = 1050;  // throttle must be below this to arm
  float timeCalibrated = -1;
  void updateArmState();
  void arm();
  int disarm();
  void logData(float dt, float errorRoll, float errorPitch, float errorYaw,
               float rollRate, float pitchRate, float yawRate);

  void updateFaults();

  int faultNumber = 0;
  bool armed = false;
  bool armSwitchWasOn = false; // previous switch position, used to detect OFF->ON edges
  bool calibrated = false;

  Servo motors[4];

  IMU imu{0.98};
  GPS gps;
  Barometer baro;
  Receiver receiver;
  LED leds;

  MotorMixer motorMixer{39, 40, 41, 42};
  PID rollAnglePID{1.1, 0, 0};
  PID pitchAnglePID{1.1, 0, 0};
  PID rollRatePID{0.45, 2, 0.02};
  PID pitchRatePID{0.45, 2, 0.02};
  PID yawRatePID{3.f, 12.f, 0};

  double requestedRollAngle;
  double requestedPitchAngle;
  double requestedYaw;
  double requestedThrottle;

  double currentRollRate;
  double currentPitchRate;
  double currentYawRate;

  double currentRollAngle;
  double currentPitchAngle;
  double currentYawAngle; // NOTE: Actually GPS heading but called "currentYawAngle" for consistency

  double currentX;
  double currentY;
  double currentZ;

  double currentVeloX;
  double currentVeloY;
  double currentVeloZ;

  float dtTelemetry[1000];
  float errorTelemetry[1000][3];
  float rateTelemetry[1000][3];
  float angleTelmetry[1000][3];

  bool alreadyLogged = false;
  bool alreadyPrinted = false;
  uint32_t lastTimeLog = 0;
  uint32_t thresholdLog = 100; // 100 ms delay between logs. Gives 10 seconds of logging
  uint32_t numberOfLogs = 0;

  float dt;
};

#endif