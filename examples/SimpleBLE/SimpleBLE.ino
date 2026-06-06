#include <EchoLib.h>

BLEServerWrapper ble("EchoBot");

void setup() {
  Serial.begin(115200);
  ble.begin();
  ble.advertise();
}

void loop() {
  ble.loop();
  
  if (ble.getStatus()) {
    String cmd = ble.readData();
    if (cmd == "PING") {
      ble.sendData("PONG");
    }
  }
  
  delay(100);
}