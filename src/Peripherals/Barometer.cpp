#include "Barometer.h"

void Barometer::begin(){
    Wire.begin(8, 9);
  // Try to initialize!
    if (!lps.begin_I2C(0x5C,&Wire)) 
        Serial.println("Failed to find LPS25 chip");

    lps.setDataRate(LPS25_RATE_25_HZ);
    update();
    initialAltitude = getAltitude(getPressure());
}
void Barometer::update(){
    sensors_event_t temp;
    sensors_event_t pressure;
    lps.getEvent(&pressure, &temp);// get pressure
    this->temperature = temp.temperature;
    this->pressure = pressure.pressure;
}

float Barometer::getAltitude(float pressure){
    this->altitude = 0.f;
    return altitude;} 
//needs implementation. 
//Note, pressure sensor can only give relative alittude to starting point. Use GPS for absolute
float Barometer::getPressure(){return pressure;}
float Barometer::getTemperature(){return temperature;}

void Barometer::printAll(){
    printf("Altitude: %f, Pressure: %f, Temperature: %f \n", 
        altitude, pressure, temperature);
}

