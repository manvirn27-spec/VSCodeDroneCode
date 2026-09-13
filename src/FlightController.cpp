#include "FlightController.h"
#include "Receiver.h"


FlightController::FlightController(){

}
FlightController::execute(){
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

    float pitchOutput = pitchRatePID(requestedPitchRate - currentPitchRate);
    float rollOutput = rollRatePID(requestedRollRate - currentRollRate);
    float yawOutput = yawRatePID(requestedYawRate - currentYawRate);

    motorMixer.spinMotors(requestedThrottle, rollOutput, pitchOutput, yawOutput);
}
FlightController::executeAutonomous(){

}
FlightController::executeHoldPosition(){

}
FlightController::executeRTH(){

}




