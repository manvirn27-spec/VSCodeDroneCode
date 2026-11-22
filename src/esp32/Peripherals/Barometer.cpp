#include "Barometer.h"

/*INAV Settings: 
    Baro weight: 0.35 altitude and speed
    GPS weight: 0.2 altitude, 0.1 speed
*/

void Barometer::begin(){ //ensure not called in flight
    Wire.begin(8, 9);
    if (!lps.begin_I2C(0x5C,&Wire)) 
        Serial.println("Failed to find LPS25 chip");
    lps.setDataRate(LPS25_RATE_25_HZ);
    for(int i = 0; i < 100; i++){
        update();
        initialPressure += getPressure();
        delay(10);
    }
    initialPressure /= 100;
}
void Barometer::update(){
    sensors_event_t temp;
    sensors_event_t pressure;
    if(lps.getEvent(&pressure, &temp)){// get pressure
        this->temperature = temp.temperature;
        this->pressure = pressure.pressure;
        float temperatureK = temperature + 273.15f;
        if(initialPressure != 0){
            float rawAltitude = temperatureK/0.0065 * (1 - std::pow(this->pressure/initialPressure, 0.1903));
            altitude = rawAltitude * 0.3 + altitude * 0.7;
        }
        numSuccess++;
    } else{
        numFail++;
    }
    //Debug print
    //if(millis() - lastTime > 1000){
    //    Serial.printf("Success: %i, Fail: %i \n", numSuccess, numFail);
    //    lastTime = millis();
    //}


}

float Barometer::getAltitude(float pressure){return altitude;} 
float Barometer::getPressure(){return pressure;}
float Barometer::getTemperature(){return temperature;}

void Barometer::printAll(){
    printf("Altitude: %f, Pressure: %f, Temperature: %f \n", 
        altitude, pressure, temperature);
}

