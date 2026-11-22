#include "FlightController.h"

void FlightController::begin(){
    delay(3000); //to let user put the drone down

    leds.begin();
    leds.indicateStartup();
    motorMixer.calibrateMotors();
    motorMixer.stopMotors();

    imu.setupIMU();
    baro.begin();
    receiver.begin();
    telem.begin();
}

void FlightController::executeAngle(){
    leds.indicateBattery(11.9);
    leds.indicateFaults(2);
    leds.indicateGPS(0);
    

    imu.update();
    receiver.update();
    baro.update();

    updateArmState();
    //Serial.println(armed);
    if (!armed) {
        motorMixer.stopMotors();
        sendTelemetry();
        return;
    }
    updateFaults();

    this->requestedRollAngle = receiver.getRoll();
    this->requestedPitchAngle = receiver.getPitch();
    this->requestedYaw = receiver.getYaw();
    this->requestedThrottle = receiver.getThrottle();

    this->currentYawRate = imu.getYawRate();
    this->currentPitchRate = imu.getPitchRate();
    this->currentRollRate = imu.getRollRate();
    this->currentRollAngle = imu.getRollAngle();
    this->currentPitchAngle = imu.getPitchAngle();

    requestedRollRate = rollAnglePID.exePID(requestedRollAngle - currentRollAngle);
    requestedPitchRate = pitchAnglePID.exePID(requestedPitchAngle - currentPitchAngle);

    pitchOutput = pitchRatePID.exePID(requestedPitchRate - currentPitchRate);
    rollOutput = rollRatePID.exePID(requestedRollRate - currentRollRate);
    yawOutput = yawRatePID.exePID(requestedYaw - currentYawRate);

    motorMixer.spinMotors(requestedThrottle, rollOutput, pitchOutput, yawOutput);
    sendTelemetry();
}

void FlightController::executeRate(){
    dt = 0;
    leds.indicateBattery(11.9);
    leds.indicateFaults(2);
    leds.indicateGPS(0);
    

    imu.update();
    receiver.update();
    baro.update();

    updateArmState();
    //Serial.println(armed);
    if (!armed) {
        motorMixer.stopMotors();
        sendTelemetry();
        return;
    }
    updateFaults();

    this->requestedRollAngle = receiver.getRoll();
    this->requestedPitchAngle = receiver.getPitch();
    this->requestedYaw = receiver.getYaw();
    this->requestedThrottle = receiver.getThrottle();

    this->currentYawRate = imu.getYawRate();
    this->currentPitchRate = imu.getPitchRate();
    this->currentRollRate = imu.getRollRate();
    this->currentRollAngle = imu.getRollAngle();
    this->currentPitchAngle = imu.getPitchAngle();

    requestedRollRate = requestedRollAngle * 6; //6 to make 300 dps
    requestedPitchRate = requestedPitchAngle * 6; //6 to make 300dps

    pitchOutput = pitchRatePID.exePID(requestedPitchRate - currentPitchRate);
    rollOutput = rollRatePID.exePID(requestedRollRate - currentRollRate);
    yawOutput = yawRatePID.exePID(requestedYaw - currentYawRate);

    //Serial.printf("Pitch error: %f, Roll error: %f, Yaw Error: %f \n", 
    //    requestedPitchRate - currentPitchRate, requestedRollRate - currentRollRate, requestedYaw, currentYawRate);
    logData(dt, requestedRollRate - currentRollAngle, requestedPitchRate - currentPitchRate,
                requestedYaw - currentYawRate, currentRollRate, currentPitchRate, currentYawRate);
                
    motorMixer.spinMotors(requestedThrottle, rollOutput, pitchOutput, yawOutput);
    sendTelemetry();
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
    bool armSwitchOn = receiver.isLinkUp() && receiver.getSA() < ARM_SWITCH_THRESHOLD;

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
    rollRatePID.resetPID();
    pitchRatePID.resetPID();
    yawRatePID.resetPID();
    armed = true;
}

int FlightController::disarm(){
    armed = false;
    motorMixer.stopMotors();
    // Telemetry still reports these while disarmed, so they must not keep the last flight values.
    requestedRollRate = requestedPitchRate = requestedYaw = 0;
    rollOutput = pitchOutput = yawOutput = 0;
    return 0;
}

void FlightController::sendTelemetry(){
    if (!telem.isDue()) {
        return;
    }

    TelemetryFields fields = {};
    fields.uptimeMs = millis();
    fields.armed = armed;
    fields.throttleUs = toFixedPoint<uint16_t>(receiver.getThrottle(), 1);
    fields.rollCentideg = toFixedPoint<int16_t>(imu.getRollAngle(), 100);
    fields.pitchCentideg = toFixedPoint<int16_t>(imu.getPitchAngle(), 100);
    fields.rollRateDecidps = toFixedPoint<int16_t>(imu.getRollRate(), 10);
    fields.pitchRateDecidps = toFixedPoint<int16_t>(imu.getPitchRate(), 10);
    fields.yawRateDecidps = toFixedPoint<int16_t>(imu.getYawRate(), 10);
    fields.rollRateSetDecidps = toFixedPoint<int16_t>(requestedRollRate, 10);
    fields.pitchRateSetDecidps = toFixedPoint<int16_t>(requestedPitchRate, 10);
    fields.yawRateSetDecidps = toFixedPoint<int16_t>(requestedYaw, 10);
    fields.rollPidOutput = toFixedPoint<int16_t>(rollOutput, 1);
    fields.pitchPidOutput = toFixedPoint<int16_t>(pitchOutput, 1);
    fields.yawPidOutput = toFixedPoint<int16_t>(yawOutput, 1);
    fields.altitudeCm = toFixedPoint<int32_t>(baro.getAltitude(0), 100);
    telem.send(fields);
}

void FlightController::updateFaults(){}

void FlightController::logData(float dt, float errorRoll, float errorPitch, float errorYaw, 
                                    float rollRate, float pitchRate, float yawRate){
    if(receiver.getSB()){
        if(!alreadyLogged){
            if(millis() - lastTimeLog > thresholdLog){
                lastTimeLog = millis();
                if (numberOfLogs >= 1000){
                    alreadyLogged = true;
                    return;
                }
                dtTelemetry[numberOfLogs] = dt;
                errorTelemetry[numberOfLogs][0] = errorRoll;
                errorTelemetry[numberOfLogs][1] = errorPitch;
                errorTelemetry[numberOfLogs][2] = errorYaw;
                rateTelemetry[numberOfLogs][0] = rollRate;
                rateTelemetry[numberOfLogs][1] = pitchRate;
                rateTelemetry[numberOfLogs][2] = yawRate;

                numberOfLogs++;

            }
        } 
        if(!alreadyPrinted && receiver.rightBumperPressed()){
            alreadyPrinted = true;
            switch(static_cast<int>(receiver.rightBumperPressed())){
                case 1:
                    Serial.println("DT:");
                    for(int i = 0; i < 1000; i++){
                        Serial.printf("%.6f \n", dtTelemetry[i]);
                    }
                    break;
                case 0:
                    Serial.println("Error:");
                    for(int i = 0; i < 1000; i++){
                        Serial.printf("%.6f, %.6f, %.6f \n",
                             errorTelemetry[i][0], errorTelemetry[i][1], errorTelemetry[i][2]);
                    }
                    break;
                case -1:
                    Serial.println("Rates:");
                    for(int i = 0; i < 1000; i++){
                        Serial.printf("%.6f, %.6f, %.6f \n",
                             rateTelemetry[i][0], rateTelemetry[i][1], rateTelemetry[i][2]);
                    }
                    break;
            }
        }   
    }
}
void FlightController::executeAutonomous(){

}
void FlightController::executeHoldPosition(){

}
void FlightController::executeRTH(){

}





