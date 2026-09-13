#ifndef PID_H
#define PID_H

#include <Arduino.h>

class PID
{
public:
  PID(double P, double I, double D, double dt);
  PID(double P, double I, double D);
  PID();
  void setP(double P);
  void setI(double I);
  void setD(double D);
  double exePID(double error);
  double exePID(double error, double dt2);

  void resetPID();

private:
  double P;
  double I;
  double D;
  double lastI;
  double lastError;
  double totalError;
  double dt;
  double timeElapsed;
  unsigned long lastTime;
};

#endif
