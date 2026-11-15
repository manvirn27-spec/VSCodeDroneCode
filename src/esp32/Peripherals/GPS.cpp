#include "GPS.h"

#include <Wire.h>
#include <math.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

GPS::GPS() : GPSSerial (2) {} //constrctor defines UART for GPSSerial

void GPS::initCompass(){
 delay(100);

 Wire1.beginTransmission(QMC5883P_ADDR);
 Wire1.write(0x0A);
 Wire1.write(0x80);
 Wire1.endTransmission();
 delay(100);

 Wire1.beginTransmission(QMC5883P_ADDR);
 Wire1.write(0x0A);
 Wire1.write(0xCF);
 Wire1.endTransmission();
 delay(10);

 Wire1.beginTransmission(QMC5883P_ADDR);
 Wire1.write(0x0B);
 Wire1.write(0x08);
 Wire1.endTransmission();
 delay(10);
}

void GPS::begin(){
  Wire1.begin(I2C_SDA, I2C_SCL);
  GPSSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  initCompass();
  Serial.println("GPS + Compass ready. Preparing...");
  }

bool GPS::update(){
  bool updated = false;
  while(GPSSerial.available() > 0){
    if(tinyGps.encode(GPSSerial.read())) updated = true;
  }
return updated;
} 

void GPS::readCompass(int16_t &x, int16_t &y, int16_t &z){
uint8_t status = 0;
 int timeout = 1000;
 while (!(status & 0x01) && timeout > 0) {
   Wire1.beginTransmission(QMC5883P_ADDR);
   Wire1.write(0x09);
   Wire1.endTransmission(false);
   Wire1.requestFrom(QMC5883P_ADDR, 1);
   if (Wire1.available()) status = Wire1.read();
   timeout--;
   delay(1);
 }


 if (timeout == 0) {
   Serial.println("WARNING: Data ready timeout - check wiring or I2C address");
   x = y = z = 0;
   return;
 }

 Wire1.beginTransmission(QMC5883P_ADDR);
 Wire1.write(0x01);
 Wire1.endTransmission(false);
 Wire1.requestFrom(QMC5883P_ADDR, 6);


 if (Wire1.available() == 6) {
   byte x_lsb = Wire1.read();
   byte x_msb = Wire1.read();
   byte y_lsb = Wire1.read();
   byte y_msb = Wire1.read();
   byte z_lsb = Wire1.read();
   byte z_msb = Wire1.read();


   x = (x_msb << 8) | x_lsb;
   y = (y_msb << 8) | y_lsb;
   z = (z_msb << 8) | z_lsb;
 } else {
   Serial.println("WARNING: Not enough bytes received from compass");
   x = y = z = 0;
 }
}

float GPS::getHeading(float x, float y){
  float heading = atan2((float)y, (float)x) * 180/PI;
  if (heading < 0){
     heading += 360.0;
  }
  return heading;
}

bool GPS::locationUpdated(){
  return tinyGps.location.isUpdated();
}

bool GPS::locationValid(){
  return tinyGps.location.isValid();
}

double GPS::locationLat(){
  return tinyGps.location.lat();
}

double GPS::locationLong(){
  return tinyGps.location.lng();
}

double GPS::altitudeMeters(){
  return tinyGps.altitude.meters();
}

uint32_t GPS::satilliteCount(){
  return tinyGps.satellites.value();
}

uint32_t GPS::charsProcessed(){
  return tinyGps.charsProcessed();
}
