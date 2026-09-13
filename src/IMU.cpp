#include "IMU.h"

void IMU::setupIMU(){
  Wire.begin(8, 9);
  if (!sox.begin_I2C(0x6B, &Wire)) {
     Serial.println("Failed to find LSM6DSOX chip");
  }
  sox.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  sox.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  sox.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  sox.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
}



void IMU::update(){
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);

  rollRate = gyro.get.y * alphaGyro + (1 - alphaGyro) * rollRate;
  pitchRate = gyro.get.x * alphaGyro + (1 - alphaGyro) * pitchRate;
  yawRate = gyro.get.z * alphaGyro + (1 - alphaGyro) * yawRate;

  rollAngle = atan(accel.get.y/sqrt(accel.get.x*accel.get.x+accel.get.z*accel.get.z))/(3.142/180);
  pitchAngle = -atan(accel.get.x/sqrt(accel.get.y*accel.get.y+accel.get.z*accel.get.z))/(3.142/180);

  //filter the rollangle
}
void IMU::calibrateIMU(){

}


double IMU::getRollRate(){

}
double IMU::getPitchRate(){

}
double IMU::getYawRate(){

}
double IMU::getRollAngle(){

}
double IMU::getPitchAngle(){

}
double IMU::getTemperature(){

}

void IMU::printValues(){
  Serial.printf("")
}