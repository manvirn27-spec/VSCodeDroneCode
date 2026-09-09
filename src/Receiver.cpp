#include "Receiver.h"

int throttle = 899;
int roll = 1500;
int pitch = 1500;
int yaw = 1500;
int aux1 = 1000;
int aux2 = 1000;
int aux3 = 1000;
int aux4 = 1000;
int aux5 = 1000;
int aux6 = 1000;

Receiver::Receiver() : crsfSerial(1) {
}

int Receiver::getThrottle() {
    return crsf.getChannel(1);
}
int Receiver::getRoll() {
    return crsf.getChannel(2);
}
int Receiver::getPitch() {
    return crsf.getChannel(3);
}
int Receiver::getYaw() {
    return crsf.getChannel(4);
}
int Receiver::getAux1() {
    return crsf.getChannel(7);
}
int Receiver::getAux2() {
    return crsf.getChannel(10);
}
int Receiver::getAux3() {
    return crsf.getChannel(8);
}
int Receiver::getAux4() {
   return  crsf.getChannel(6);
}
int Receiver::getAux5() {
    return crsf.getChannel(5);
}
int Receiver::getAux6() {
    return crsf.getChannel(6);
}

void Receiver::updateReceiver() {
    crsf.update();
}
void Receiver::begin(int TX, int RX){
    crsfSerial.begin(CRSF_BAUDRATE, SERIAL_8N1, RX, TX);  
    crsf.begin(crsfSerial);

}