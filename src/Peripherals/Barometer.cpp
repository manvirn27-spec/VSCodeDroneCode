#include "Barometer.h"

void Barometer::begin(){
    Wire.begin(8, 9);
  // Try to initialize!
    if (!lps.begin_I2C(0x5C,&Wire)) 
        Serial.println("Failed to find LPS25 chip");

    lps.setDataRate(LPS25_RATE_25_HZ);
    update();
    initialPressure = getPressure();
}
void Barometer::update(){
    sensors_event_t temp;
    sensors_event_t pressure;
    lps.getEvent(&pressure, &temp);// get pressure

    this->temperature = temp.temperature;
    this->pressure = pressure.pressure;

    float temperatureK = temperature + 273.15f;
    altitude = temperatureK/0.0065 * (1 - std::pow(this->pressure/initialPressure, 0.1903));
}

float Barometer::getAltitude(float pressure){
    return altitude;
} 
float Barometer::getPressure(){return pressure;}
float Barometer::getTemperature(){return temperature;}

void Barometer::printAll(){
    printf("Altitude: %f, Pressure: %f, Temperature: %f \n", 
        altitude, pressure, temperature);
}

