#include "IMU.h"
#include <cmath>

IMU::IMU(double alphaGyro){
  this->alphaGyro = alphaGyro;
}

void IMU::setupIMU(){
  Wire.begin(8, 9);
  if (!imu.begin_I2C(0x6B, &Wire)) {
     Serial.println("Failed to find LSM6DSOX chip");
  }
  imu.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);
  imu.setGyroRange(LSM6DS_GYRO_RANGE_2000_DPS );
  imu.setAccelDataRate(LSM6DS_RATE_833_HZ);
  imu.setGyroDataRate(LSM6DS_RATE_833_HZ);
}



void IMU::update(){
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  imu.getEvent(&accel, &gyro, &temp);
  temperature = temp.temperature;

  // 1. Get raw rates in degrees per second
  float rawRollRate  =  gyro.gyro.y * 180.f/M_PI;
  float rawPitchRate = -gyro.gyro.x * 180.f/M_PI;
  float rawYawRate   = -gyro.gyro.z * 180.f/M_PI;

  rollRate = rawRollRate * alphaGyro + (1 - alphaGyro) * rollRate;
  pitchRate = rawPitchRate * alphaGyro + (1 - alphaGyro) * pitchRate;
  yawRate = rawYawRate * alphaGyro + (1 - alphaGyro) * yawRate;
  //Debug print
  //Serial.println(yawRate);

  pitchAngle = -atan(accel.acceleration.y/sqrt(accel.acceleration.x*accel.acceleration.x+accel.acceleration.z*accel.acceleration.z))/(3.142/180);
  rollAngle = -atan(accel.acceleration.x/sqrt(accel.acceleration.y*accel.acceleration.y+accel.acceleration.z*accel.acceleration.z))/(3.142/180);

  //filter the rollangle using Kalman
}
void IMU::calibrateIMU(){

}


double IMU::getRollRate(){
  return rollRate;
}
double IMU::getPitchRate(){
  return pitchRate;
}
double IMU::getYawRate(){
  return yawRate;
}
double IMU::getRollAngle(){
  return rollAngle;
}
double IMU::getPitchAngle(){
  return pitchAngle;
}
double IMU::getTemperature(){
  return temperature;
}

void IMU::printValues(){
  Serial.printf("Roll Rate: %f, Pitch Rate: %f, Yaw Rate: %f, Roll Angle: %f, Pitch Angle: %f \n", 
    rollRate, pitchRate, yawRate, rollAngle, pitchAngle);
}