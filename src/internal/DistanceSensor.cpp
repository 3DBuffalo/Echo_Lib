#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(int trig, int echo, boolean metric) {
    _trig   = trig;
    _echo   = echo;
    _metric = metric;

    this->filled = false;

    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

float DistanceSensor::getDistance(){
    float raw = this->getDistanceRaw();

    // Only process valid (non-zero) readings
    if(raw > 0.00){
        this->readings[this->ind] = raw;
        this->ind++;
    }

    // Once we have collected 10 valid readings
    if(this->ind >= 5){
        float sum = 0;
        for(int i = 0; i < 5; i++){
            sum += this->readings[i];
        }
        
        // Update the last known average
        this->lastAverage = sum / 5.0;

        // Reset for the next batch
        memset(this->readings, 0, sizeof(this->readings));
        this->ind = 0;
    }

    // Return the last successful average
    // If no batch has ever finished, this will return 0.0
    return this->lastAverage;
}

float DistanceSensor::getDistanceRaw() {
    // Fire a 10µs pulse on the trigger pin
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trig, LOW);

    // Measure the echo pulse duration (timeout after 30 ms to avoid hanging)
    long duration = pulseIn(_echo, HIGH, 30000);

    // Speed of sound: 343 m/s = 0.0343 cm/µs
    // Distance (cm) = (duration / 2) * 0.0343
    // Convert to mm: multiply cm by 10
    float distanceMM = (duration / 2.0f) * 0.343f;  // µs → mm

    if (_metric) {
        return distanceMM;
    } else {
        // 1 mm = 0.0393701 inches
        return distanceMM * 0.0393701f;
    }
}