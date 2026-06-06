#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>

// ===== Register Definitions =====
#define BMI270_I2C_ADDR      0x68
#define WHO_AM_I_REG         0x00
#define INTERNAL_STATUS_REG  0x21
#define COMMAND_REG          0x7E
#define PWR_CONF_REG         0x7C
#define PWR_CTRL_REG         0x7D
#define INIT_CTRL_REG        0x59
#define INIT_ADDR_0_REG      0x5B
#define INIT_ADDR_1_REG      0x5C
#define INIT_DATA_REG        0x5E
#define ACC_CONF_REG         0x40
#define ACC_RANGE_REG        0x41
#define GYRO_CONF_REG        0x42
#define GYRO_RANGE_REG       0x43

// Data registers
#define ACC_X_LSB 0x0C
#define ACC_Y_LSB 0x0E
#define ACC_Z_LSB 0x10
#define GYRO_X_LSB 0x12
#define GYRO_Y_LSB 0x14
#define GYRO_Z_LSB 0x16

// External config blob
extern const uint8_t bmi270_config_file[];

class IMU {
public:
    IMU();

    void begin();     // Init IMU (keeps your exact code)
    void reset();     // Reset angles + biases

    // ===== Simple Beginner Methods =====
    // Accelerometer (m/s^2)
    float getAccelXms();
    float getAccelYms();
    float getAccelZms();

    // Gyroscope (absolute angle 0–360 degrees)
    float getGyroXdeg();
    float getGyroYdeg();
    float getGyroZdeg();

    // Accelerometer G-force
    float getForceX();
    float getForceY();
    float getForceZ();

    // Raw sensor values
    int16_t getRawAccelX();
    int16_t getRawAccelY();
    int16_t getRawAccelZ();
    int16_t getRawGyroX();
    int16_t getRawGyroY();
    int16_t getRawGyroZ();

    // Orientation
    void getOrientation(float &roll, float &pitch, float &yaw);

    // Calibration
    void calibrateAccel();
    void calibrateGyro();

private:
    uint8_t readRegister(uint8_t reg);
    int16_t readRegister16(uint8_t reg);        // signed 16-bit, 2-byte burst
    void readAllRaw(int16_t out[6]);            // coherent accel+gyro sample
    void writeRegister(uint8_t reg, uint8_t value);
    void softReset();
    void initBMI270();
    void updateAngles();                        // integrate + fuse (call often)

    // internal helpers
    float lowPassFilter(float current, float previous, float alpha = 0.8f);

    // biases
    float accel_bias[3];
    float gyro_bias[3];

    // fused orientation
    float roll, pitch, yaw;

    // integrated absolute gyro angles (0–360 wrap)
    float angleX, angleY, angleZ;

    bool first_update;          // seed the filter from accel on first sample
    unsigned long last_time;
};

#endif