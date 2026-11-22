#ifndef BAROMETER_H
#define BAROMETER_H

#include <Arduino.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <cmath>

class Barometer{
    public:
        void update();
        void begin();

        float getAltitude(float pressure); //Units: meters above the startup altitude
        float getPressure(); //Units: hPa
        float getTemperature(); //Units: C

        void printAll();
    private:
        Adafruit_LPS25 lps;
        float initialPressure;
        float altitude;
        float pressure;
        float temperature;

};

#endif
