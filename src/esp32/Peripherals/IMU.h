#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Math.h>
#include <Adafruit_LSM6DSOX.h>
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
    float alphaGyro;
    
    float rollRate;
    float pitchRate;
    float yawRate;
    float rollAngle;
    float pitchAngle;

    float rateCalibrationRoll;
    float rateCalibrationPitch;
    float rateCalibrationYaw;

    float temperature;
};

#endif