#include "FlightController.h"

FlightController::FlightController() : motorMixer(motors) {

}
void FlightController::execute(){
    imu.update();
    receiver.updateReceiver();

    updateArmState();
    if (!armed) {
        motorMixer.stopMotors();
        return;
    }

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

bool FlightController::isArmed(){
    return armed;
}

/**
 * Reads the arm switch (AUX1) and updates the armed state.
 *
 * Arming requires all of the following, to avoid a prop spinning up unexpectedly:
 *  - receiver link is up
 *  - the arm switch transitions from OFF to ON (holding it ON at power-up will not arm)
 *  - throttle is at minimum at the moment of that transition
 *
 * Disarming happens immediately when the switch goes OFF or the link is lost.
 */
void FlightController::updateArmState(){
    bool armSwitchOn = receiver.isLinkUp() && receiver.getAux1() > ARM_SWITCH_THRESHOLD;

    if (armed) {
        if (!armSwitchOn) {
            disarm();
        }
    } else {
        bool switchJustTurnedOn = armSwitchOn && !armSwitchWasOn;
        bool throttleLow = receiver.getThrottle() < ARM_THROTTLE_MAX;
        if (switchJustTurnedOn && throttleLow) {
            arm();
        }
    }

    armSwitchWasOn = armSwitchOn;
}

void FlightController::arm(){
    // Clear any accumulated integral / derivative history so the first
    // control outputs after arming start from a clean state.
    rollAnglePID.resetPID();
    pitchAnglePID.resetPID();
    yawAnglePID.resetPID();
    rollRatePID.resetPID();
    pitchRatePID.resetPID();
    yawRatePID.resetPID();
    armed = true;
}

void FlightController::disarm(){
    armed = false;
    motorMixer.stopMotors();
}
void FlightController::executeAutonomous(){

}
void FlightController::executeHoldPosition(){

}
void FlightController::executeRTH(){

}




