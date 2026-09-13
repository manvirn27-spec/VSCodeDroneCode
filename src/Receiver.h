#ifndef Receiver_H
#define Receiver_H

#include <Arduino.h>
#include <AlfredoCRSF.h>

class Receiver{
  public:
    int getThrottle();
    int getRoll();
    int getPitch();
    int getYaw();
    int getAux1();
    int getAux2();
    int getAux3();
    int getAux4();
    int getAux5();
    int getAux6();

    void updateReceiver();
    void begin(int TX, int RX);
    bool isLinkUp();

    Receiver(); 
  private:
    HardwareSerial crsfSerial;
    AlfredoCRSF crsf;

    int throttle;
    int roll;
    int pitch;
    int yaw;
    int aux1;
    int aux2;
    int aux3;
    int aux4;
    int aux5;
    int aux6;
};

#endif