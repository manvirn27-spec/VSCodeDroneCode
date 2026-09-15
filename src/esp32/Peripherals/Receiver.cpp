#include "Receiver.h"

#include <AlfredoCRSF.h>
#include <HardwareSerial.h>

#define PIN_RX 5
#define PIN_TX 4

// Set up a new Serial object
HardwareSerial crsfSerial(1);
AlfredoCRSF crsf;

 int ROLL_CHANNEL = 1;
 int PITCH_CHANNEL = 2;
 int THROTTLE_CHANNEL = 3;
 int YAW_CHANNEL = 4;

 int SC_CHANNEL = 8;
 int SF_CHANNEL = 6;
 int SA_CHANNEL = 7;
// const int S1_CHANNEL = 5;

 int SD_CHANNEL = 9;
 int SB_CHANNEL = 10;
 int S1_CHANNEL = 11;
 int S2_CHANNEL = 12;

 float PITCH_ANGLE_LIMIT = 50.0f;
 float ROLL_ANGLE_LIMIT = 50.0f;
// TODO: change
 float YAW_ROTATION_MAX_SPEED_DEG_PER_SEC = 360.0f;

void Receiver::update()
{
    crsf.update();
}

void Receiver::begin()
{

    Serial.begin(115200);
    crsfSerial.begin(CRSF_BAUDRATE, SERIAL_8N1, PIN_RX, PIN_TX);
    crsf.begin(crsfSerial);
}

int Receiver::getRoll()
{
    int roll = crsf.getChannel(ROLL_CHANNEL);

    return ((roll - 1500) / 500.0f) * ROLL_ANGLE_LIMIT;
}

int Receiver::getPitch()
{
    int pitch = crsf.getChannel(PITCH_CHANNEL);
    return ((pitch - 1500) / 500.0f) * PITCH_ANGLE_LIMIT;
}

int Receiver::getYaw()
{
    int yaw = crsf.getChannel(YAW_CHANNEL);
    return ((yaw - 1500) / 500.0f) * YAW_ROTATION_MAX_SPEED_DEG_PER_SEC;
}

// Manvir idk what units you want for this
int Receiver::getThrottle()
{
    return crsf.getChannel(THROTTLE_CHANNEL);
}

// I forgot which of the switches have a neutral state which is why its a bunch of if statements
int Receiver::getSC()
{
    int SC = crsf.getChannel(SC_CHANNEL);
    if (SC <= 1100)
    {
        return 1;
    }
    else if (SC >= 1900)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int Receiver::getSA()
{
    int SA = crsf.getChannel(SA_CHANNEL);
    if (SA <= 1100)
    {
        return 1;
    }
    else if (SA >= 1900)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

bool Receiver::getSF()
{
    int SF = crsf.getChannel(SF_CHANNEL);
    if (SF <= 1100)
    {
        return false;
    }
    else if (SF >= 1900)
    {
        return true;
    }
    else
    {
        return false;
    }
}

float Receiver::getS1()
{
    int S1 = crsf.getChannel(S1_CHANNEL);
    return ((S1 - 1500) / 500.0f);
}

float Receiver::getS2()
{
    int S2 = crsf.getChannel(S2_CHANNEL);
    return ((S2 - 1500) / 500.0f);
}

int Receiver::getSB()
{
    int SB = crsf.getChannel(SB_CHANNEL);
    if (SB <= 1100)
    {
        return 1;
    }
    else if (SB >= 1900)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int Receiver::getSD()
{
    int SD = crsf.getChannel(SD_CHANNEL);
    if (SD <= 1100)
    {
        return 1;
    }
    else if (SD >= 1900)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

bool Receiver::isLinkUp() {
    return crsf.isLinkUp(); // Calls the underlying AlfredoCRSF link check
}

/*

Channel 8 = SC: up -> 1000, neutral -> 1503, down -> 2000
Channel 6 = SF: pressed = 2000, released = 1000
Channel 7 = SA: up -> 1000, down = 2000
Channel 5 = S1: left -> 1000, right -> 2000 (no in between)

Channel 12 = S2: all the way left: 1000, all the way right -> 2000
Channel 11 = S1: all the way left: 1000, all the way right -> 2000
Channel 10 = SB: up -> 1000, down -> 2000
Channel 9 = SD up -> 1000, down -> 2000

Channel 3 = throttle: up -> 2008, down -> 991
Channel 1 = roll: left -> 990, right -> 2011
Channel 2 = pitch: up -> 2011, down -> 990
Channel 4 = yaw: left -> 990, right -> 2011
neutral = 1500
*/