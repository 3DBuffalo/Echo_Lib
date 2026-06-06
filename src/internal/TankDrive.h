//TankDrive.h
#ifndef TANK_DRIVE_H
#define TANK_DRIVE_H

#include "Motor.h"
#include "MotorControllers.h"


class TankDrive {
  private:
    MotorControllers& controller;

    uint8_t leftLead;
    uint8_t rightLead;

    uint8_t extraLeft[2];  
    uint8_t extraLeftCount;  

    uint8_t extraRight[2];  
    uint8_t extraRightCount;

  public:
    
    TankDrive(MotorControllers& controller, uint8_t leftLead, uint8_t rightLead,
                   uint8_t* extraLeftIDs = nullptr, uint8_t extraLeftCount = 0,
                   uint8_t* extraRightIDs = nullptr, uint8_t extraRightCount = 0);

    void drive(int x, int y);

    void setBrake();

    void setCoast();

};

#endif //TANK_DRIVE_H