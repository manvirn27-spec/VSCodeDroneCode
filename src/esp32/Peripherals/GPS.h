#ifndef GPS_H
#define GPS_H

#include <Wire.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <stdint.h>

class GPS{
  public:
    GPS();
    bool update();
    void initCompass();
    void begin();
    void readCompass(int16_t &x, int16_t &y, int16_t &z);
    float getHeading(float x, float y);

    bool locationUpdated();
    bool locationValid();
    double locationLat();
    double locationLong();
    double altitudeMeters();
    uint32_t satilliteCount();
    uint32_t charsProcessed();
  private:
    TinyGPSPlus tinyGps;
    HardwareSerial GPSSerial;

    static constexpr int RXPin = 16, TXPin = 17;
    static constexpr uint32_t GPSBaud = 9600;
    static constexpr int I2C_SDA = 5;
    static constexpr int I2C_SCL = 18;
    static constexpr int QMC5883P_ADDR = 0x2C;
};

#endif
