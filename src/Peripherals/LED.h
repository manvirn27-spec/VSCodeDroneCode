#ifndef LED_H
#define LED_H

#include <Arduino.h>

class LED{
    public:
        void begin();

        void indicateStartup();
        void indicateBattery(float voltage);
        void indicateGPS(int satelites);
        void indicateFaults(int faultNumber);
    private:
        int pinNums[5] = {16, 15, 14, 13, 12};
    // Startup sequence state variables
        unsigned long lastStepTimeStartup = 0;
        int currentLedIndexStartup = 0;
        int currentIntervalStartup = 500; // 0.5 seconds
    //Fault sequence state variables
        int blinkCountFault = 0;        // Keeps track of current blinks in cycle
        bool ledStateFault = false;     // Current hardware state (HIGH/LOW)
        unsigned long lastTimeFault = 0; // Timestamp for non-blocking delays
};

#endif