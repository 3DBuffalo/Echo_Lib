#include <EchoLib.h>

MotorControllers motors;
TankDrive drive(motors, 1, 2);  // Use motor IDs 1 and 2

void setup() {
  Serial.begin(115200);
  drive.setBrake();
  Serial.println("Tank Drive Ready");
}

void loop() {
  // Move forward
  Serial.println("Forward");
  drive.drive(0, 60);
  delay(2000);
  
  // Turn right in place
  Serial.println("Spin Right");
  drive.drive(50, 0);
  delay(1000);
  
  // Arc left while moving forward
  Serial.println("Arc Left Forward");
  drive.drive(-25, 50);
  delay(2000);
  
  // Move backward
  Serial.println("Backward");
  drive.drive(0, -50);
  delay(2000);
  
  // Stop
  Serial.println("Stop");
  drive.drive(0, 0);
  delay(1000);
}