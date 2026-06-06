#include "BLEConnection.h"

// CharacteristicCallbacks implementation
CharacteristicCallbacks::CharacteristicCallbacks(BLEServerWrapper* wrapper) 
    : _wrapper(wrapper) {
}

void CharacteristicCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    String rxValue = String(pCharacteristic->getValue().c_str());
    if (rxValue.length() > 0) {
        _wrapper->_receivedData = rxValue;
        Serial.print("Received Value: ");
        Serial.println(_wrapper->_receivedData);
    }
}

// BLEServerWrapper implementation
BLEServerWrapper::BLEServerWrapper(const String& bluetoothID) 
    : _bluetoothID(bluetoothID),
      _server(nullptr),
      _service(nullptr),
      _txCharacteristic(nullptr),
      _rxCharacteristic(nullptr),
      _isConnected(false),
      _autoReconnect(true),
      _initialized(false),
      _receivedData("") {
}

BLEServerWrapper::~BLEServerWrapper() {
    if (_initialized) {
        disconnect();
        BLEDevice::deinit(true);
    }
}

void BLEServerWrapper::begin() {
    if (_initialized) return;
    
    Serial.println("Initializing BLE...");
    
    // Create the BLE Device
    BLEDevice::init(_bluetoothID.c_str());
    
    // Create the BLE Server
    _server = BLEDevice::createServer();
    if (_server == nullptr) {
        Serial.println("Error: Failed to create BLE server");
        return;
    }
    _server->setCallbacks(this);
    
    // Create the BLE Service
    _service = _server->createService(SERVICE_UUID);
    if (_service == nullptr) {
        Serial.println("Error: Failed to create BLE service");
        return;
    }
    
    // Create TX Characteristic (for transmitting/notifying data to client)
    _txCharacteristic = _service->createCharacteristic(
        TX_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY
    );
    
    if (_txCharacteristic == nullptr) {
        Serial.println("Error: Failed to create TX characteristic");
        return;
    }
    
    // Add descriptor for notifications
    _txCharacteristic->addDescriptor(new BLE2902());
    
    // Create RX Characteristic (for receiving/writing data from client)
    _rxCharacteristic = _service->createCharacteristic(
        RX_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    
    if (_rxCharacteristic == nullptr) {
        Serial.println("Error: Failed to create RX characteristic");
        return;
    }
    
    // Set callback for when data is received
    _rxCharacteristic->setCallbacks(new CharacteristicCallbacks(this));
    
    // Start the service
    _service->start();
    
    _initialized = true;
    
    Serial.println("BLE Server initialized successfully");
    Serial.print("Device name: ");
    Serial.println(_bluetoothID);
}

void BLEServerWrapper::advertise() {
    if (!_initialized) {
        begin();
    }
    
    startAdvertising();
}

void BLEServerWrapper::startAdvertising() {
    if (_server != nullptr && _initialized) {
        _server->getAdvertising()->start();
        Serial.println("==============================");
        Serial.println("BLE Advertising started");
        Serial.print("Device name: ");
        Serial.println(_bluetoothID);
        Serial.println("Waiting for a client connection...");
        Serial.println("==============================");
    } else {
        Serial.println("ERROR: Cannot start advertising - not initialized");
    }
}

void BLEServerWrapper::disconnect() {
    if (_server != nullptr && _isConnected) {
        _server->disconnect(_server->getConnId());
    }
    
    if (_server != nullptr) {
        _server->getAdvertising()->stop();
    }
    
    _isConnected = false;
    Serial.println("BLE Disconnected");
}

bool BLEServerWrapper::getStatus() const {
    return _isConnected;
}

void BLEServerWrapper::autoReconnect(bool enable) {
    _autoReconnect = enable;
}

void BLEServerWrapper::sendData(const String& data) {
    if (_txCharacteristic != nullptr && _isConnected) {
        _txCharacteristic->setValue(data.c_str());
        _txCharacteristic->notify();
        Serial.print("Sent: ");
        Serial.println(data);
    } else {
        if (!_isConnected) {
            Serial.println("Cannot send data: Not connected");
        } else {
            Serial.println("Cannot send data: TX characteristic is null");
        }
    }
}

String BLEServerWrapper::readData() {
    String data = _receivedData;
    _receivedData = "";  // Clear after reading
    return data;
}

void BLEServerWrapper::loop() {
    // Handle auto-reconnect when disconnected
    if (_autoReconnect && !_isConnected && _initialized) {
        static unsigned long lastReconnectAttempt = 0;
        unsigned long currentMillis = millis();
        
        // Try to reconnect every 5 seconds
        if (currentMillis - lastReconnectAttempt >= 5000) {
            lastReconnectAttempt = currentMillis;
            Serial.println("Auto-reconnecting...");
            startAdvertising();
        }
    }
}

void BLEServerWrapper::onConnect(BLEServer* pServer) {
    _isConnected = true;
    Serial.println("*** Client connected! ***");
}

void BLEServerWrapper::onDisconnect(BLEServer* pServer) {
    _isConnected = false;
    Serial.println("*** Client disconnected! ***");
    
    // Restart advertising if auto-reconnect is enabled
    if (_autoReconnect) {
        delay(500);  // Small delay before restarting
        startAdvertising();
    }
}