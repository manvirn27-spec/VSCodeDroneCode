#ifndef FlightController_H
#define FlightController_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <cstdint>

#include "MotorMixer.h"
#include "PID.h"

//#include "../Peripherals/GPS.h"
#include "../Peripherals/IMU.h"
#include "../Peripherals/Receiver.h"
#include "../Peripherals/Barometer.h"
#include "../Peripherals/LED.h"
#include "../Peripherals/TelemComms.h"

class FlightController
{
public:
  void begin();
  void calibrate(Receiver &receiver);

  //void executeAngle();
  void executeRate(IMU &imu, Receiver &receiver, LED &leds);

  void executeHoldPosition();
  void executeAutonomous();
  void executeRTH();

  bool isArmed();

private:
  static const int ARM_SWITCH_THRESHOLD = 0; 
  static const int ARM_THROTTLE_MAX = 1050;
  long timeCalibrated = -1;
  void updateArmState(Receiver &receiver);
  void arm();
  int disarm();

  //BROKEN:
  //void logData(float dt, float errorRoll, float errorPitch, float errorYaw,
  //             float rollRate, float pitchRate, float yawRate);

private:
  void updateFaults();

  int faultNumber = 0;
  bool armed = false;
  bool armSwitchWasOn = false;
  bool calibrated = false;
  bool armedOnce = false;

  Servo motors[4];
  IMU imu{0.501};
  //GPS gps;

  MotorMixer motorMixer;
  PID rollAnglePID{1.1, 0, 0};
  PID pitchAnglePID{1.1, 0, 0};
  PID rollRatePID{1.3f, 1.2f, 0.003f}; //1.15, 1.2, 0.003
  PID pitchRatePID{1.3f, 1.2f, 0.003f};
  PID yawRatePID{1.5f, 3.5f, 0}; //1.5, 3.5, 0

  double requestedRollAngle;
  double requestedPitchAngle;
  double requestedYaw;
  double requestedThrottle;

  float requestedRollRate;
  float requestedPitchRate;
  float rollOutput;
  float pitchOutput;
  float yawOutput;

  double currentRollRate;
  double currentPitchRate;
  double currentYawRate;

  double currentRollAngle;
  double currentPitchAngle;
  double currentYawAngle; // NOTE: Actually GPS heading but called "currentYawAngle" for consistency

  double currentX;
  double currentY;
  double currentZ;

  
  // double currentX;
  // double currentY;
  // double currentZ;
  double currentVeloX;
  double currentVeloY;
  double currentVeloZ;

  float dtTelemetry[1000];
  float errorTelemetry[1000][6];
  float rateTelemetry[1000][3];
  float angleTelmetry[1000][3];

  bool alreadyLogged = false;
  bool alreadyPrinted = false;
  uint32_t lastTimeLog = 0;
  uint8_t thresholdLog = 10; // 100 ms delay between logs. Gives 10 seconds of logging
  uint16_t numberOfLogs = 0;

  float dt;
  uint32_t dt_us;
  float lastTime = micros();

};

#endif