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
        void begin(); //ENSURE NOT CALLED IN FLIGHT. uses delay()

        float getAltitude(float pressure); //Units: Feet, relative to start
        float getPressure(); //Units: inHg
        float getTemperature(); //Units: F

        void printAll();
    private:
        Adafruit_LPS25 lps;
        float initialPressure;
        float altitude;
        float pressure;
        float temperature;


        //test variables
        uint16_t numSuccess = 0;
        uint16_t numFail = 0;
        uint32_t lastTime = 0;

};

#endif
