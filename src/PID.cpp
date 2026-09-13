#include "PID.h"

PID::PID(double P, double I, double D, double dt)
{
  this->P = P;
  this->I = I;
  this->D = D;
  lastError = 0;
  lastI = 0;
  this->dt = dt;
  this->timeElapsed = 0;
  this->totalError = 0;
  this->lastTime = 0;
}

PID::PID(double P, double I, double D)
{
  this->P = P;
  this->I = I;
  this->D = D;
  lastError = 0;
  lastI = 0;
  this->timeElapsed = 0;
  this->totalError = 0;
  this->lastTime = 0;
  // dt = 0.004;
}

PID::PID()
{
  P = 0;
  I = 0;
  D = 0;
  lastError = 0;
  lastI = 0;
  this->timeElapsed = 0;
  this->totalError = 0;
  this->lastTime = 0;
  // dt = 0.004;
}

void PID::setP(double P)
{
  this->P = P;
}
void PID::setI(double I)
{
  this->I = I;
}
void PID::setD(double D)
{
  this->D = D;
}

double PID::exePID(double error, double dt2)
{
  double result = 0;
  double pResult = error * P;
  double dResult = (error - lastError) / dt2;
  double iResult = error * I + lastI;
  result = pResult + dResult + iResult;

  lastError = error;
  return result;
}

/**
 * Executes the PID controller with the given error.
 * @param error The error to use for the PID calculation.
 * @return The result of the PID calculation.
 */
double PID::exePID(double error)
{
  unsigned long now = millis();
  double dt = (now - lastTime) / 1000.0; // seconds since last call
  lastTime = now;
  if (dt <= 0) dt = 0.001; // avoid divide-by-zero on the first / back-to-back calls

  double result = 0;
  double pResult = error * P;
  double dResult = D * (error - lastError) / dt;

  totalError = totalError + error * dt;
  double iResult = totalError * I;
  result = pResult + dResult + iResult;

  lastError = error;
  return result;
}
void PID::resetPID()
{
  lastI = 0;
  lastError = 0;
}
