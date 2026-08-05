#include <EchoLib.h>

Timer blinkTimer;
const int LED_PIN = 2;
bool ledState = false;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    blinkTimer.start();
}

void loop() {
    // Blink every 500ms without blocking
    if (blinkTimer.get() >= 500) {
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
        blinkTimer.reset();
        blinkTimer.start();
    }
    // Other code runs continuously
    // Read sensors, check buttons, etc.
}
