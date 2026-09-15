#ifndef Receiver_H
#define Receiver_H

class Receiver
{
public:
  int getThrottle();
  int getRoll();
  int getPitch();
  int getYaw();

  bool getSF();
  int getSA();
  int getSC();
  int getSD();
  int getSB();

  float getS1();
  float getS2();

  void update();
  void begin();
  bool isLinkUp();

private:

  int ROLL_CHANNEL;
  int PITCH_CHANNEL;
  int THROTTLE_CHANNEL;
  int YAW_CHANNEL;
  int SC_CHANNEL;
  int SF_CHANNEL;
  int SA_CHANNEL;
  // const int S1_CHANNEL = 5;
  int SD_CHANNEL;
  int SB_CHANNEL;
  int S1_CHANNEL;
  int S2_CHANNEL;

  int throttle;
  int roll;
  int pitch;
  int yaw;
  bool SF;
  int SA;
  int SC;
  int SD;
  int SB;
  float S1;
  float S2;
};

#endif