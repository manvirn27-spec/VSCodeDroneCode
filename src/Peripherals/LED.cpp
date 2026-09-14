#include "LED.h"

//EVERYTHING MUST BE NONBLOCKING. NO DELAYS
void LED::begin(){
    for (int i = 0; i < 5; i++){
        pinMode(pinNums[i], OUTPUT);
        digitalWrite(pinNums[i], LOW);
    }
}

void LED::indicateBattery(float voltage){
    if(voltage > 11.88){
        //full - 75%
    } else if(voltage > 11.55){
        //75% - 50%
    } else if(voltage > 11.28){
        //50% - 25%
    } else if(voltage > 9.90){
        //25% - 0% (should never be near zero)
    } else{
        for(int i = 0; i < 5; i++){
            digitalWrite(pinNums[i], LOW);
        }
    }
        //emergency reduce power
}
void LED::indicateFaults(int faultNumber){
    //blink number of faults quickly, then 2 second pause. MUST BE NONBLOCKING
}
void LED::indicateGPS(int numSatelites){
    //Indicate number of satelites
}
void LED::indicateStartup(){
    unsigned long currentMillis = millis();

    // Determine current interval based on whether we are moving or pausing
    // Steps 0-4 take 100ms each (500ms total). Step 5 is the 500ms pause.
    unsigned long currentIntervalStartup = (currentLedIndexStartup == 5) ? 500 : 100;

    if (currentMillis - lastStepTimeStartup >= currentIntervalStartup) {
        lastStepTimeStartup = currentMillis;

        // Turn OFF all LEDs
        for (int i = 0; i < 5; i++) {
            digitalWrite(pinNums[i], LOW);
        }

        if (currentLedIndexStartup < 5) {
            // Turn ON the current LED moving bottom to top
            digitalWrite(pinNums[currentLedIndexStartup], HIGH);
        }
        // When currentLedIndexStartup == 5, all LEDs remain LOW for the pause duration

        // Advance to next step (0 through 5)
        currentLedIndexStartup++;
        if (currentLedIndexStartup > 5) {
            currentLedIndexStartup = 0;
        }
    }
}