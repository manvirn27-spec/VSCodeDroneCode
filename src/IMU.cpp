#include "IMU.h"
#include <Math.h>
#include <Wire.h>

IMU::IMU(double alpha){
  this->alphaGyro = alpha;
}

double IMU::getRollRate(){return rollRate;}
double IMU::getPitchRate(){return pitchRate;}
double IMU::getYawRate(){return yawRate;}
double IMU::getRollAngle(){return rollAngle;}
double IMU::getPitchAngle(){return pitchAngle;}

void IMU::setupIMU(){
  Wire.setClock(400000);
  Wire.begin(26, 27);
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void IMU::calibrateIMU(){
  int rateCalibrationNumber;

  for (rateCalibrationNumber=0; rateCalibrationNumber<2000; rateCalibrationNumber ++) {
    calculateValues();
    rateCalibrationRoll+=getRollRate();
    rateCalibrationPitch+=getPitchRate();
    rateCalibrationYaw+=getYawRate();
    delay(1);
  }
  rateCalibrationRoll/=2000;
  rateCalibrationPitch/=2000;
  rateCalibrationYaw/=2000;
}
void IMU::calculateValues(){
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission(); 
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B); 
  Wire.write(0x8);
  Wire.endTransmission();                                                   
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX=Wire.read()<<8 | Wire.read();
  int16_t GyroY=Wire.read()<<8 | Wire.read();
  int16_t GyroZ=Wire.read()<<8 | Wire.read();
  rollRate=(float)GyroX/65.5;
  pitchRate=(float)GyroY/65.5;
  yawRate=(float)GyroZ/65.5;
  double AccX=(float)AccXLSB/4096;
  double AccY=(float)AccYLSB/4096;
  double AccZ=(float)AccZLSB/4096;
  double filteredRollRate = alphaGyro * filteredRollRate + (1 - alphaGyro) * rollRate;
  double filteredPitchRate = alphaGyro * filteredPitchRate + (1 - alphaGyro) * pitchRate;

  rollRate = filteredRollRate;
  pitchRate = filteredPitchRate;

  rollAngle=atan(AccY/sqrt(AccX*AccX+AccZ*AccZ))*1/(3.142/180);
  pitchAngle=-atan(AccX/sqrt(AccY*AccY+AccZ*AccZ))*1/(3.142/180);
}

void IMU::printValues(){
    printf("Roll Rate: %f, PitchRate: %f, YawRate: %f, RollAngle: %f, Pitch Angle: %f \n", 
    rollRate, pitchRate, yawRate, rollAngle, pitchAngle);
}