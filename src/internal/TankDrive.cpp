#include "TankDrive.h"

TankDrive::TankDrive(MotorControllers& controller, uint8_t leftLead, uint8_t rightLead,
                               uint8_t* extraLeftIDs, uint8_t extraLeftCount,
                               uint8_t* extraRightIDs, uint8_t extraRightCount)
    : controller(controller), leftLead(leftLead), rightLead(rightLead), 
      extraLeftCount(extraLeftCount), extraRightCount(extraRightCount) 
{
    for (uint8_t i = 0; i < 2; i++) {
        if (extraLeftIDs != nullptr && i < extraLeftCount) {
            extraLeft[i] = extraLeftIDs[i];
        } else {
            extraLeft[i] = 0;
        }
    }

    for (uint8_t i = 0; i < 2; i++) {
        if (extraRightIDs != nullptr && i < extraRightCount) {
            extraRight[i] = extraRightIDs[i];
        } else {
            extraRight[i] = 0;
        }
    }
}

void TankDrive::drive(int x, int y){
  //x is left right, y is forward back 
  int left = x + y;
  int right = y - x;

  controller.set(leftLead, left);
  for(uint8_t i = 0; i < extraLeftCount; i++){
    controller.set(extraLeft[i],left);
  }

  controller.set(rightLead, right);
  for(uint8_t i = 0; i < extraRightCount; i++){
    controller.set(extraRight[i],right);
  }
}

void TankDrive::setBrake(){
  controller.setBrake();
}

void TankDrive::setCoast(){
  controller.setCoast();
}