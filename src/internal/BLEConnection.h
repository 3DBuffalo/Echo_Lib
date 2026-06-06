#ifndef BLE_CONNECTION_H
#define BLE_CONNECTION_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Forward declaration
class BLEServerWrapper;

// Callback class for characteristic writes
class CharacteristicCallbacks : public BLECharacteristicCallbacks {
public:
    CharacteristicCallbacks(BLEServerWrapper* wrapper);
    void onWrite(BLECharacteristic* pCharacteristic) override;
private:
    BLEServerWrapper* _wrapper;
};

class BLEServerWrapper : public BLEServerCallbacks {
public:
    BLEServerWrapper(const String& bluetoothID);
    ~BLEServerWrapper();
    
    void begin();  // Initialize BLE
    void advertise();
    void disconnect();
    bool getStatus() const;
    void autoReconnect(bool enable);
    void sendData(const String& data);
    String readData();
    void loop();  // Call this in main loop for auto-reconnect

private:
    String _bluetoothID;
    BLEServer* _server;
    BLEService* _service;
    BLECharacteristic* _txCharacteristic;  // For sending (notify)
    BLECharacteristic* _rxCharacteristic;  // For receiving (write)
    bool _isConnected;
    bool _autoReconnect;
    bool _initialized;
    String _receivedData;
    
    // UUIDs
    static constexpr const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
    static constexpr const char* TX_CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8";
    static constexpr const char* RX_CHARACTERISTIC_UUID = "a4488e9d-3c11-42fd-89c8-3ae3a57395e6";
    
    // BLEServerCallbacks overrides
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
    
    // Helper methods
    void startAdvertising();
    
    // Allow callback to access private members
    friend class CharacteristicCallbacks;
};

#endif // BLE_CONNECTION_H