#include "robot_communication.hpp"
#include "Arduino.h"

RobotCommunication::RobotCommunication()
{
    service_UUID_ = BLEUUID("a9d058d7-8a5e-4241-981a-56b1b3c242fc");
    message_characteristic_UUID_ = BLEUUID("c803fc6c-d736-42cd-8547-c24d7b899f55");
    BLEServer* pServer_ = NULL;
    BLECharacteristic* pCharacteristic_ = NULL;
    bool connectedClient_ = false;
    std::string currentInstructions_ = "";
}

void RobotCommunication::setup()
{

    Serial.println("Starting BLE work!");

    // Create the BLE Device
    BLEDevice::init("ESP32");

    // Create the BLE Server
    pServer_ = BLEDevice::createServer();

    pServer_->setCallbacks(new PamiServerCallbacks(&connectedClient_));

    // Create the BLE Service
    BLEService* pService = pServer_->createService(service_UUID_);

    // Create a BLE Characteristic
    pCharacteristic_ = pService->createCharacteristic(
        message_characteristic_UUID_,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );

    // Descriptor 2902 is not required when using NimBLE as it is automatically added based on the characteristic properties
    pCharacteristic_->addDescriptor(new BLE2902());

    pCharacteristic_->setCallbacks(new PamiServerCharacteristicCallbacks(&currentInstructions_));
    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(service_UUID_);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting for client connections to notify...");
}

int RobotCommunication::amountConnectedClient()
{
    return connectedClient_;
}

std::string RobotCommunication::currentInstruction()
{
    return currentInstructions_;
}

PamiServerCharacteristicCallbacks::PamiServerCharacteristicCallbacks(std::string* currentInstructions)
{
    currentInstructions_ = currentInstructions;
}

void PamiServerCharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic_)
{
    (*currentInstructions_) = pCharacteristic_->getValue();
    //Serial.print("Written value: ");
    //Serial.println((*currentInstructions_).c_str());
}

PamiServerCallbacks::PamiServerCallbacks(bool* connectedClients)
{
    connectedClients_ = connectedClients;
}

void PamiServerCallbacks::onConnect(BLEServer* pServer_)
{
    (*connectedClients_) = true;
    Serial.println("Client connected");
}

void PamiServerCallbacks::onDisconnect(BLEServer* pServer_)
{
    (*connectedClients_) = false;
    Serial.println("Client disconnected");
}
