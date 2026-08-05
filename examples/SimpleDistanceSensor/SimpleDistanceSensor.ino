#include <EchoLib.h>

const int TRIG_PIN = 3;
const int ECHO_PIN = 42;

DistanceSensor sensor(TRIG_PIN, ECHO_PIN); // metric = true (mm) by default

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("DistanceSensor example starting...");
}

void loop() {
    float distance = sensor.getDistance();
    Serial.print("Distance: ");
    Serial.print(distance);
}
