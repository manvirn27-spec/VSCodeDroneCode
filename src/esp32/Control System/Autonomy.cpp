#include "Autonomy.h"

void Autonomy::begin(GPS &gps){
    this->gps = &gps;
    updateHomePos();
}

void Autonomy::updateHomePos(){
    if(gps->locationValid()){
        homeLat = gps->locationLat();
    } else{
        Serial.println("NO VALID HOME LOCATION");
        
    }
}
void Autonomy::updateTargetPos(){}

void Autonomy::update(){}

float Autonomy::getResultingThrottle(){return 0.0f;}
float Autonomy::getResultingPitch(){return 0.0f;}
float Autonomy::getResultingRoll(){return 0.0f;}
float Autonomy::getResultingYaw(){return 0.0f;}

