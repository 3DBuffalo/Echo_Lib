#include <EchoLib.h>

MotorControllers motorControllers;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  motorControllers.set(1, 80);    // Motor 1 at 80% forward
  motorControllers.set(3, -60);   // Motor 3 at 60% reverse
  delay(1000);// Runs for 1 second
  motorControllers.set(1,0);//stops motors
  motorControllers.set(3,0);
  delay(1000);//waits 1 second
}
