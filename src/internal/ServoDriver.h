#ifndef SERVODRIVER_H
#define SERVODRIVER_H

#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

class ServoDriver {
private:
    Adafruit_PWMServoDriver driver;
    static const int SERVOMIN = 150; // Minimum pulse length count (0 degrees)
    static const int SERVOMAX = 600; // Maximum pulse length count (180 degrees)

    int angleToPulseLength(int angle);

public:
    ServoDriver(); // Constructor
    void init();   // Initialize with default frequency
    void init(int freq); // Initialize with custom frequency
    void setAngle(int servo, int angle); // Set angle for a specific servo
    void setZero(); // Set all servos to 0
};

#endif