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
    
    void begin();
    void readCompass(int16_t &x, int16_t &y, int16_t &z);
    float getHeading(int16_t x, int16_t y);

    bool locationUpdated();
    bool locationValid();
    double locationLat();
    double locationLong();
    double altitudeMeters();
    uint32_t satilliteCount();
    uint32_t charsProcessed();

  private:
    void initCompass();
    TinyGPSPlus tinyGps;
    HardwareSerial GPSSerial;

    static constexpr int RXPin = 3, TXPin = 2;
    static constexpr uint32_t GPSBaud = 9600;
    static constexpr int I2C_SDA = 6;
    static constexpr int I2C_SCL = 7;
    static constexpr int QMC5883P_ADDR = 0x2C;

    const float MEGAOFFSET = 47; //degrees compass
};

#endif
