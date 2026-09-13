#include "FlightController.h"
#include "Receiver.h"


FlightController::FlightController() : motorMixer(motors) {

}
void FlightController::execute(){
    imu.calculateValues();
    receiver.updateReceiver();

    this->requestedRollAngle = receiver.getRoll();
    this->requestedPitchAngle = receiver.getPitch();
    this->requestedYaw = receiver.getYaw();
    this->requestedThrottle = receiver.getThrottle();

    this->currentYawRate = imu.getYawRate();
    this->currentPitchRate = imu.getPitchRate();
    this->currentRollRate = imu.getRollRate();
    this->currentRollAngle = imu.getRollAngle();
    this->currentPitchAngle = imu.getPitchAngle();

    float requestedRollRate = rollAnglePID.exePID(requestedRollAngle - currentRollAngle);
    float requestedPitchRate = pitchAnglePID.exePID(requestedPitchAngle - currentPitchAngle);

    float pitchOutput = pitchRatePID.exePID(requestedPitchRate - currentPitchRate);
    float rollOutput = rollRatePID.exePID(requestedRollRate - currentRollRate);
    float yawOutput = yawRatePID.exePID(requestedYaw - currentYawRate);

    motorMixer.spinMotors(requestedThrottle, rollOutput, pitchOutput, yawOutput);
}
void FlightController::executeAutonomous(){

}
void FlightController::executeHoldPosition(){

}
void FlightController::executeRTH(){

}




