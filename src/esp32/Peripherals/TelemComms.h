#include <cstdint>
#ifndef TELEMCOMMS_H
#define TELEMCOMMS_H

class TelemComms{
    public:
        void begin();
        void send();
        void getData();
    private:
        struct __attribute(packed()) telem{

            //if we want to optimze, we can pack packetNumber and uptimeMs into 1 uint32_t
            uint16_t packetNumber; //enough for ~1.8h of uptime
            uint32_t uptimeMs; //enough for 1000h of uptime, can't use uint16_t bc only enough for 1min of uptime then
            uint16_t rollPIDOutput;
            uint16_t pitchPIDOutput;
            uint16_t requestedRoll;
            uint16_t requestedPitch;
            uint16_t currentRollRate;
            uint16_t currentPitchRate;
        

            uint8_t batteryMv;
            uint32_t gpsLongitude;
            uint32_t gpsLatitude;
            uint32_t heading;

        }
}


#endif