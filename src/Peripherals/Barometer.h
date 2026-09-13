#ifndef BAROMETER_H
#define BAROMETER_H

#include <Arduino.h>
#include <LPS.h>
class Barometer{
    public:
        void update();
        void begin();

        float getAltitude(); //Units: Feet
        float getPressure(); //Units: inHg
        float getTemperature(); //Units: F

        void printAll();
    private:
        LPS baro;

        float altitude;
        float pressure;
        float temperature;

};

#endif
