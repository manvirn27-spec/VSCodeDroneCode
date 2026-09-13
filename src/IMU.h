#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "IMU.h"
#include <Math.h>
#include <adafruit_lsmd6s.h>
#include <Wire.h>



class IMU{
  public:
    IMU(double alpha);
    void update();
    void setupIMU();
    void calibrateIMU();

    double getRollRate();
    double getPitchRate();
    double getYawRate();
    double getRollAngle();
    double getPitchAngle();
    double getTemperature();

    void printValues();

  private:
    Adafruit_LSM6DSOX imu;
    double alphaGyro = 0.9;
    
    double rollRate;
    double pitchRate;
    double yawRate;
    double rollAngle;
    double pitchAngle;

    double alphaGyro;

    double rateCalibrationRoll;
    double rateCalibrationPitch;
    double rateCalibrationYaw;
};

#endif