#ifndef TELEMCOMMS_H
#define TELEMCOMMS_H

class TelemComms{
    public:
        void begin();
        void send();
        void getData();
    private:
        struct __attribute(packed()) telem{

            uint16_t rollPIDOutput;
            uint16_t pitchPIDOutput;
            uint16_t requestedRoll;
            uint16_t requestedPitch;
            uint16_t currentRollRate;
            uint16_t currentPitchRate
        

            byte batteryVoltage;
            uint32_t gpsLatitude;
            uint32_t gpsLatitude;
            uint32_t heading;

        }
}


#endif