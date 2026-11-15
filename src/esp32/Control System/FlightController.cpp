#include "FlightController.h"

void FlightController::begin()
{
    
    motorMixer.begin();
    motorMixer.stopMotors();
    
    delay(3000); // to let user put the drone down
    Serial.println("Beginning");
}

void FlightController::calibrate(Receiver &receiver)
{
    // receiver.printChannel(5);
    // receiver.printChannel(13);
    // receiver.printChannel(14);
    // receiver.printChannel(15);
    // receiver.printChannel(16);
    // Serial.printf("armed:  %i \n", isArmed());
    // Serial.printf("Throttle down: %i \n", receiver.getThrottle() <= 1050);
    if ((receiver.getThrottle() <= 1050 && isArmed() == false && armedOnce == false && receiver.rightBumperPressed() && calibrated == false))
    {
        if (timeCalibrated == -1)
        {
            timeCalibrated = 0;
            // Serial.print("beginning calibration");
            motorMixer.calibrateMotors();
        }
        else if (timeCalibrated == 0)
        {
            timeCalibrated = millis();
            Serial.print(timeCalibrated);
        }
        else if (millis() - timeCalibrated >= 3000 && calibrated == false)
        {

            calibrated = true;
            // Serial.print("calibrated");
        }
    }
}

/*
void FlightController::executeAngle()
{
    leds.indicateBattery(11.9);
    leds.indicateFaults(2);
    leds.indicateGPS(0);

    imu.update();
    receiver.update();
    updateArmState();


    this->currentYawRate = imu.getYawRate();
    this->currentPitchRate = imu.getPitchRate();
    this->currentRollRate = imu.getRollRate();
    this->currentRollAngle = imu.getRollAngle(); //TODO: IMU class needs filtering
    this->currentPitchAngle = imu.getPitchAngle();

    this->requestedRollAngle = receiver.getRoll();
    this->requestedPitchAngle = receiver.getPitch();
    this->requestedYaw = receiver.getYaw();
    this->requestedThrottle = receiver.getThrottle();

    float requestedRollRate = rollAnglePID.exePID(requestedRollAngle - currentRollAngle, dt);
    float requestedPitchRate = pitchAnglePID.exePID(requestedPitchAngle - currentPitchAngle, dt);

    if(requestedThrottle < 1000){
        pitchRatePID.resetPID();
        rollRatePID.resetPID();
        yawRatePID.resetPID();
    }
    float pitchOutput = pitchRatePID.exePID(requestedPitchRate - currentPitchRate, dt);
    float rollOutput = rollRatePID.exePID(requestedRollRate - currentRollRate, dt);
    float yawOutput = yawRatePID.exePID(requestedYaw - currentYawRate, dt);
    
    logData(this->dt, requestedRollRate, requestedPitchRate,
        requestedYaw, currentRollRate, currentPitchRate, currentYawRate);

    updateFaults();
    if (!armed)
    {
        motorMixer.stopMotors();
        return; //----------------NOTE: EVERYTHING UNDER DOESN'T RUN IF NOT ARMED
    }

    motorMixer.spinMotors(requestedThrottle, rollOutput, pitchOutput, yawOutput);

    while (micros() - lastTime < 4000)
    {
    }

    dt_us = (micros() - lastTime);
    lastTime = micros();
    this->dt = dt_us / 1000000.f;
}
*/
void FlightController::executeRate(IMU &imu, Receiver &receiver, LED &leds)
{
    leds.indicateBattery(11.9);
    leds.indicateFaults(2);
    leds.indicateGPS(0);

    updateArmState(receiver);
    // Serial.println(armed);

    this->currentYawRate = imu.getYawRate();
    this->currentPitchRate = imu.getPitchRate();
    this->currentRollRate = imu.getRollRate();
    
    this->currentRollAngle = imu.getRollAngle(); //TODO: IMU class needs filtering
    this->currentPitchAngle = imu.getPitchAngle();

    this->requestedRollAngle = receiver.getRoll();
    this->requestedPitchAngle = receiver.getPitch();
    this->requestedYaw = receiver.getYaw();
    this->requestedThrottle = receiver.getThrottle();

    float requestedRollRate = requestedRollAngle * 3;   // 6 to make 300 dps
    float requestedPitchRate = requestedPitchAngle * 3; // 6 to make 300dps

    if(requestedThrottle < 1000){
        pitchRatePID.resetPID();
        rollRatePID.resetPID();
        yawRatePID.resetPID();
    }
    float pitchOutput = pitchRatePID.exePID(requestedPitchRate - currentPitchRate, dt);
    float rollOutput = rollRatePID.exePID(requestedRollRate - currentRollRate, dt);
    float yawOutput = yawRatePID.exePID(requestedYaw - currentYawRate, dt);
    
    //logData(this->dt, requestedRollRate, requestedPitchRate,
    //    requestedYaw, currentRollRate, currentPitchRate, currentYawRate);

    updateFaults();
    if (!armed)
    {
        motorMixer.stopMotors();
        return; //----------------NOTE: EVERYTHING UNDER DOESN'T RUN IF NOT ARMED
    }

    motorMixer.spinMotors(requestedThrottle, rollOutput, pitchOutput, yawOutput);

    while (micros() - lastTime < 4000) {}

    dt_us = (micros() - lastTime);
    lastTime = micros();
    this->dt = dt_us / 1000000.f;
}

bool FlightController::isArmed()
{
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

void FlightController::updateArmState(Receiver &receiver)
{
    bool armSwitchOn = receiver.isLinkUp() && receiver.getSA() < ARM_SWITCH_THRESHOLD;

    if (armed)
    {
        if (!armSwitchOn)
        {
            disarm();
        }
    }
    else
    {
        bool switchJustTurnedOn = armSwitchOn && !armSwitchWasOn;
        bool throttleLow = receiver.getThrottle() < ARM_THROTTLE_MAX;
        if (switchJustTurnedOn && throttleLow)
        {
            arm();
        }
    }

    armSwitchWasOn = armSwitchOn;
}

void FlightController::arm()
{
    // Clear any accumulated integral / derivative history so the first
    // control outputs after arming start from a clean state.
    rollAnglePID.resetPID();
    pitchAnglePID.resetPID();
    rollRatePID.resetPID();
    pitchRatePID.resetPID();
    yawRatePID.resetPID();
    armed = true;
    armedOnce = true;
}

int FlightController::disarm()
{
    armed = false;
    motorMixer.stopMotors();
    return 0;
}

void FlightController::updateFaults() {}

/* BROKEN, needs fixing
void FlightController::logData(float dt, float requestedRoll, float requestedPitch, float requestedYaw,
                               float rollRate, float pitchRate, float yawRate)
{
    if (receiver.getSB() == -1)
    {
        if (!alreadyLogged)
        {
            if (millis() - lastTimeLog > thresholdLog)
            {
                lastTimeLog = millis();
                if (numberOfLogs >= 1000)
                {
                    alreadyLogged = true;
                    return;
                }
                dtTelemetry[numberOfLogs] = dt;

                errorTelemetry[numberOfLogs][0] = requestedRoll;
                errorTelemetry[numberOfLogs][1] = rollRate;
                errorTelemetry[numberOfLogs][2] = requestedPitch;
                errorTelemetry[numberOfLogs][3] = pitchRate;
                errorTelemetry[numberOfLogs][4] = requestedYaw;
                errorTelemetry[numberOfLogs][5] = yawRate;

                rateTelemetry[numberOfLogs][0] = rollRate;
                rateTelemetry[numberOfLogs][1] = pitchRate;
                rateTelemetry[numberOfLogs][2] = yawRate;

                numberOfLogs++;
            }
        }
    }
    if (receiver.rightBumperPressed() && !alreadyPrinted)
    {
        alreadyPrinted = true;
        switch (static_cast<int>(receiver.getSD()))
        {
        case 1:
            Serial.println("DT:");
            for (int i = 0; i < 1000; i++)
            {
                Serial.printf("%.6f \n", dtTelemetry[i]);
            }
            break;
        case 0:
            Serial.println("Error:");
            for (int i = 0; i < 1000; i++)
            {
                Serial.printf("R_Req: %.6f, R_Act: %.6f | P_Req: %.6f, P_Act: %.6f | Y_Req: %.6f, Y_Act: %.6f\n",
                      errorTelemetry[i][0], errorTelemetry[i][1], 
                      errorTelemetry[i][2], errorTelemetry[i][3], 
                      errorTelemetry[i][4], errorTelemetry[i][5]);
            }
            break;
        case -1:
            Serial.println("Rates:");
            for (int i = 0; i < 1000; i++)
            {
                Serial.printf("%.6f, %.6f, %.6f \n",
                              rateTelemetry[i][0], rateTelemetry[i][1], rateTelemetry[i][2]);
            }
            break;
        }
    }
}
    */
void FlightController::executeAutonomous()
{
}
void FlightController::executeHoldPosition()
{
}
void FlightController::executeRTH()
{
}
