#ifndef AUTONOMY_H
#define AUTONOMY_H

#include "../Peripherals/GPS.h"
#include "../Peripherals/Barometer.h"

class Autonomy{
    public:
        void begin(GPS &gps);
        void updateHomePos();
        void updateTargetPos();

        void update();

        float getResultingThrottle();
        float getResultingPitch();
        float getResultingRoll();
        float getResultingYaw();
    private:
        GPS *gps = nullptr;

        float homeLat;
        float homeLong;
        
        float throttleOut;
        float pitchAngleOut;
        float rollAngleOut;
        float yawRateOut;
};


#endif