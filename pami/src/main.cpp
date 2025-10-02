#include <Servo.h>
#include "motion.hpp"
#include "BLEDevice.h"
#include "robot_communication.hpp"

/*
 * Description:
 * Example for setting the minimal and maximal angle.
 */

Motion testMotion;
RobotCommunication testComm;

void setup()
{
    Serial.begin(115200);

    delay(5000);

    //testMotion.setup();
    testComm.setup();

}

void loop()
{
    Serial.print("Connected clients: ");
    Serial.println(testComm.amountConnectedClient());
    //testMotion.auxilary_action();
    //testMotion.movement(0.1, 90, 1);

    if (testComm.amountConnectedClient()) {
        Serial.println(testComm.currentInstruction().c_str());
    }

    delay(1000); // Delay a second between loops.
}
