#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

#include <Arduino.h>

class DistanceSensor {
public:
    /**
     * @param trig   Trigger pin number
     * @param echo   Echo pin number
     * @param metric If true (default), getDistance() returns millimeters.
     *               If false, returns inches.
     */
    DistanceSensor(int trig, int echo, boolean metric = true);

    /**
     * Fires the sensor and returns the measured distance.
     * @return Distance in mm (metric=true) or inches (metric=false)
     */
    float getDistance();

    /**
     * Gets raw sensor values
    */
    float getDistanceRaw();

private:
    int _trig;
    int _echo;
    boolean _metric;
    Timer timer;
    boolean filled;
    int ind = 0;
    int readings[5];
    float lastAverage;
};

#endif // DISTANCE_SENSOR_H