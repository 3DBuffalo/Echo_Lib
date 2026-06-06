// mecanumDrive.cpp
#include "MecanumDrive.h"

// Constructor implementation
MecanumDrive::MecanumDrive(MotorControllers& controller, uint8_t FrontL, uint8_t FrontR, uint8_t BackL, uint8_t BackR)
    : controller(controller), frontLeft(FrontL), frontRight(FrontR), backLeft(BackL), backRight(BackR), isFieldCentric(false) {}

void MecanumDrive::drive(int x, int y, int turn) {
  int FL = y + x + turn;
  int FR = y - x - turn;
  int BL = y - x + turn;
  int BR = y + x - turn;

  controller.set(frontLeft,FL);
  controller.set(frontRight,FR);
  controller.set(backLeft,BL);
  controller.set(backRight,BR);
}

void MecanumDrive::setBrake() {
  controller.getMotorById(frontLeft)->setBrakeMode();
  controller.getMotorById(frontRight)->setBrakeMode();
  controller.getMotorById(backLeft)->setBrakeMode();
  controller.getMotorById(backRight)->setBrakeMode();
}

void MecanumDrive::setCoast() {
  controller.getMotorById(frontLeft)->setCoastMode();
  controller.getMotorById(frontRight)->setCoastMode();
  controller.getMotorById(backLeft)->setCoastMode();
  controller.getMotorById(backRight)->setCoastMode();
}