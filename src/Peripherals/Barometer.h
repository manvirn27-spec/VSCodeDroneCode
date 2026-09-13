#ifndef BAROMETER_H
#define BAROMETER_H

#include <Arduino.h>
#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

class Barometer{
    public:
        void update();
        void begin();

        float getAltitude(float pressure); //Units: Feet
        float getPressure(); //Units: inHg
        float getTemperature(); //Units: F

        void printAll();
    private:
        Adafruit_LPS25 lps;
        float initialAltitude;
        float altitude;
        float pressure;
        float temperature;

};

#endif
