/*#ifndef AUTONOMY_H
#define AUTONOMY_H

#include "GPS.h"
#include "Barometer.h"

class Autonomy{
    public:
        void begin();
        void updateHomePos();
        void updateTargetPos();

        void update();

        float getResultingThrottle();
        float getResultingPitch();
        float getResultingRoll();
        float getResultingYaw();
    private:
        GPS gps{};
        Barometer baro;
}


#endif

*/