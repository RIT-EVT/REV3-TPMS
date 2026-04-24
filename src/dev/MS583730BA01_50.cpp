#include <core/utils/time.hpp>
#include <dev/MS583730BA01_50.hpp>
#include <sys/types.h>

namespace time = core::time;
namespace io = core::io;

namespace TPMS::dev {

MS5837::MS5837(io::I2C& i2c_bus) : i2c(i2c_bus){
}

bool MS5837::init() {
    uint8_t resetCmd = 0x1E; // Transmit resetCmd to I2C_ADDRESS
    i2c.write(I2C_ADDRESS,&resetCmd, 1);
    time::wait(200);
    // read 7 prom calibration commands
    for (uint8_t i = 0; i < 7; i++) {
        uint8_t promCmd = 0xA0 + (i*2);
        uint8_t buffer[2];
        i2c.write(I2C_ADDRESS, &promCmd, 1); // Transmit promCMD
        i2c.read(I2C_ADDRESS, buffer, 2); // Receive two bytes into buffer
        calibrationData[i] = (buffer[0] << 8) | buffer[1];
    }
}

/**
 * Get the pressure from the MS5837
 * @return pressure in mbar
 */
float MS5837::readPressure() {
    uint8_t buffer[3];
    uint8_t adcReadCmd = 0x00;

    // Get Raw pressure (D1)
    uint8_t cmdD1 = 0x48;
    i2c.write(I2C_ADDRESS, &cmdD1, 1);
    time::wait(10);
    i2c.write(I2C_ADDRESS, &adcReadCmd, 1);
    i2c.read(I2C_ADDRESS, buffer, 3);
    uint32_t D1 = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);

    // Get Raw temperature (D2) required for the pressure math
    uint8_t cmdD2 = 0x58;
    i2c.write(I2C_ADDRESS, &cmdD2, 1);
    time::wait(10);
    i2c.write(I2C_ADDRESS, &adcReadCmd, 1);
    i2c.read(I2C_ADDRESS, buffer, 3);
    uint32_t D2 = (buffer[0] << 16) | (buffer[1] << 8) | (buffer[2]);

    // Compensation Math
    const uint16_t C1 = calibrationData[1];
    const uint16_t C2 = calibrationData[2];
    const uint16_t C3 = calibrationData[3];
    const uint16_t C4 = calibrationData[4];
    const uint16_t C5 = calibrationData[5];
    const uint16_t C6 = calibrationData[6];

    int32_t dT = D2 - ((uint32_t)C5 << 8);
    int32_t TEMP = 2000 + (((int64_t)dT * C6) >> 23);

    int64_t OFF = ((int64_t)C2 << 16) + (((int64_t)C4 * dT) >> 7);
    int64_t SENS = ((int64_t)C1 << 15) + (((int64_t)C3 * dT) >> 8);

    // Second order compensation for cold temperatures
    if (TEMP <= 2000) {
        int64_t OFF2 = 3 * ((TEMP - 2000) * (TEMP - 2000)) >> 1;
        int64_t SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) >> 3;

        if (TEMP <= -1500) {
            OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
            SENS2 = SENS2 + 4 * ((TEMP + 1500) * (TEMP + 1500));
        }

        OFF -= OFF2;
        SENS -= SENS2;
    }
    int32_t P = ((((int64_t)D1 * SENS) >> 21) - OFF) >> 13;
    return P / 10.0f; // Return final pressure in mbar
}

/**
 * Gets the temperature from the MS5837
 * idk if we need this currently not implemented
 * @return temperature in degrees Celsius
 */
float MS5837::readTemperature() {
    //int64_t Ti = 3 * ((int64_t)dT * dT) >> 33;
    return 0.0;
}
} // namespace TPMS::dev
