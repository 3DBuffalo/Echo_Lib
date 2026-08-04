#ifndef DRIVER_STATION_H
#define DRIVER_STATION_H

#include <Arduino.h>

class DriverStation {
public:
    DriverStation();

    /**
     * Appends an integer value to the outgoing data packet.
     */
    void putInt(int data, int ID);

    /**
     * Appends a float value to the outgoing data packet.
     */
    void putFloat(float data, int ID);

    /**
     * Appends a boolean value to the outgoing data packet.
     */
    void putBool(boolean data, int ID);

    /**
     * Appends a String value to the outgoing data packet.
     */
    void putString(String data, int ID);

    /**
     * Returns the data string at the given section index from the last
     * processed packet.
     */
    String getData(int ID);

    /*
    * Returns the data string as is
    */
    String getDataRaw();

    /**
     * Processes incoming/outgoing data — call once per loop iteration.
     */
    void process(String recievedData);

    /**
    * Returns controller button data on button: "X"
    */
    bool getX();

    /**
    * Returns controller button data on button: "Y"
    */
    bool getY();

    /**
    * Returns controller button data on button: "A"
    */
    bool getA();

    /**
    * Returns controller button data on button: "B"
    */
    bool getB();

    /**
    * Returns controller button data on button: "Left Bumper"
    */
    bool getBumperL();

    /**
    * Returns controller button data on button: "Right Bumper"
    */
    bool getBumperR();

    /**
    * Returns controller button data on button: "D-pad Up"
    */
    bool getDU();

    /**
    * Returns controller button data on button: "D-pad Down"
    */
    bool getDD();

    /**
    * Returns controller button data on button: "D-pad Left"
    */
    bool getDL();

    /**
    * Returns controller button data on button: "D-pad Right"
    */
    bool getDR();

    /**
    * Returns controller data on "Left Joystick X"
    */
    float getLX();

    /**
    * Returns controller data on "Left Joystick Y"
    */
    float getLY();

    /**
    * Returns controller data on "Right Joystick X"
    */
    float getRX();

    /**
    * Returns controller data on "Right Joystick Y"
    */
    float getRY();

    /**
    * Returns controller data on "Right Trigger"
    */
    float getRT();

    /**
    * Returns controller data on "Right Trigger"
    */
    float getLT();

    /**
    * Returns if driver station is enabled/disabled
    */
    int getState();
private:
    String data = "";

    bool getControllerBtn(String btn);

    float getControllerAxis(int axis);

    void putData(String d, int ID);

    /**
     * Extracts the raw value for slot ID (1-10) from an arbitrary packet
     * string, without reading or modifying the stored data. Used by
     * process() to check whether an incoming packet actually has real
     * (non-empty) content for a given slot before deciding to adopt it.
     */
    String extractSlotValue(const String &src, int ID);
};

#endif // DRIVER_STATION_H