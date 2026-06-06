#include <EchoLib.h>

ServoDriver servoDriver;

void setup() {
    servoDriver.init();
    
    // Start in "home" position
    servoDriver.setZero();
    delay(1000);
    
    Serial.begin(115200);
    Serial.println("Robot arm ready!");
}

void loop() {
    // Move to "grab" position
    servoDriver.setAngle(0, 45);  // Base rotation
    servoDriver.setAngle(1, 90);  // Shoulder
    servoDriver.setAngle(2, 135); // Elbow
    delay(2000);
    
    // Move to "lift" position
    servoDriver.setAngle(1, 45);  // Lift shoulder
    servoDriver.setAngle(2, 90);  // Straighten elbow
    delay(2000);
    
    // Return home
    servoDriver.setZero();
    delay(2000);
}