#ifndef IMU_H
#define IMU_H

#include <Arduino.h>

class IMU{
  public:
    IMU(double alpha);
    void calculateValues();
    void setupIMU();
    void calibrateIMU();

    double getRollRate();
    double getPitchRate();
    double getYawRate();
    double getRollAngle();
    double getPitchAngle();

    void printValues();

  private:
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