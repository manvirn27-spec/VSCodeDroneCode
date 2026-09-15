#ifndef Receiver_H
#define Receiver_H

class Receiver
{
public:
  float getThrottle();
  float getRoll();
  float getPitch();
  float getYaw();

  bool getSF();
  float getSA();
  float getSC();
  float getSD();
  float getSB();

  float getS1();
  float getS2();

  void update();
  void begin();
  bool isLinkUp();

  void printAll();

private:

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

  float throttle;
  float roll;
  float pitch;
  float yaw;
  bool SF;
  float SA;
  float SC;
  float SD;
  float SB;
  float S1;
  float S2;
};

#endif