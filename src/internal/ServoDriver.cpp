#include "ServoDriver.h"

ServoDriver::ServoDriver() : driver(Adafruit_PWMServoDriver()) {}

void ServoDriver::init() {
    Wire.begin(17, 18);
    driver.begin();
    driver.setOscillatorFrequency(27000000);
    driver.setPWMFreq(60); // Default frequency 60 Hz
    delay(10);
}

void ServoDriver::init(int freq) {
    Wire.begin(17, 18);
    driver.begin();
    driver.setOscillatorFrequency(27000000);
    driver.setPWMFreq(freq); // Custom frequency
    delay(10);
}

int ServoDriver::angleToPulseLength(int angle) {
    // Ensure the angle is within the range 0 to 180 degrees
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // Calculate pulse length based on the angle
    return SERVOMIN + (angle / 180.0) * (SERVOMAX - SERVOMIN);
}

void ServoDriver::setAngle(int servo, int angle) {
    if (servo < 0 || servo > 9) {
        Serial.println("Invalid servo channel");
        return;
    }
    int pulse = angleToPulseLength(angle);
    driver.setPWM(servo, 0, pulse);
}

void ServoDriver::setZero() {
    for (int i = 0; i < 10; i++) {
        driver.setPWM(i, 0, 0);
    }
}