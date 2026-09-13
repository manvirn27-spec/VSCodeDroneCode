#include "Barometer.h"

void Barometer::begin(){
    baro.init();
    baro.enableDefault();
}
void Barometer::update(){
    this->pressure = baro.readPressureInchesHg();
    this->altitude = baro.pressureToAltitudeFeet(pressure);
    this->temperature = baro.readTemperatureF();
}

float Barometer::getAltitude(){return altitude;}
float Barometer::getPressure(){return pressure;}
float Barometer::getTemperature(){return temperature;}

void Barometer::printAll(){
    printf("Altitude: %f, Pressure: %f, Temperature: %f \n", 
        altitude, pressure, temperature);
}

