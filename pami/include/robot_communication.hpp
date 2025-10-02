#ifndef ROBOT_COMMUNICATION_HPP
#define ROBOT_COMMUNICATION_HPP

#include "BLEDevice.h"
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

/**
 * @class RobotCommunication
 * @brief Create a BLE client to communicate with the robot
 */
class RobotCommunication
{
public:
    /**
     * @brief A constructor
     */
    RobotCommunication();

    /**
     * @brief Setup the BLE client and connect to server
     */
    void setup();

    /**
     * @brief Return amount connected clients
     * @return amount connected clients
     */
    int amountConnectedClient();

    /**
     * @brief Return current instruction from client
     * @return current instruction from client
     */
    std::string currentInstruction();

    /**
     * @brief The UUID of the service and characteristic we wish to create
     */
    BLEUUID service_UUID_;
    BLEUUID message_characteristic_UUID_;

    /**
     * @brief Model of the server
     */
    BLEServer* pServer_;

    /**
     * @brief Model of the characteristic
     */
    BLECharacteristic* pCharacteristic_ = NULL;

    /**
     * @brief Variable representing wether a client is connected
     */
    bool connectedClient_;

    /**
     * @brief Most recent instruction sent by client
     */
    std::string currentInstructions_;

protected:

};

class PamiServerCallbacks : public BLEServerCallbacks
{
public:
    /**
     * @brief A constructor
     * @param connectedClients_ a pointer towards the boolean that tracks wether a client is connected
     */
    PamiServerCallbacks(bool* connectedClients_);

    /**
     * @brief Called when a client connect to the server
     * @param pServer_ a model of the server
     */
    void onConnect(BLEServer* pServer_);

    /**
     * @brief Called when a client disconnect to the server
     * @param pServer_ a model of the server
     */
    void onDisconnect(BLEServer* pServer_);

    /**
     * @brief A pointer towards the boolean that tracks wether a client is connected
     */
    bool* connectedClients_;


};

class PamiServerCharacteristicCallbacks : public BLECharacteristicCallbacks
{
public:
    /**
     * @brief A constructor
     * @param currentInstructions_ a pointer towards the std::string that saves the most recent instruction
     */
    PamiServerCharacteristicCallbacks(std::string* currentInstructions_);

    /**
     * @brief Called when a client write on the characteristic
     * @param pCharacteristic_ a model of the caracteristic
     */
    void onWrite(BLECharacteristic* pCharacteristic_);

    /**
     * @brief A pointer towards the std::string that saves the most recent instruction
     */
    std::string* currentInstructions_;
};
#endif // ROBOT_COMMUNICATION_HPP
