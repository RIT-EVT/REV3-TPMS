//
// Created by ppani on 4/18/2026.
//

#ifndef REV3_TPMS_MS583730BA01_50_HPP
#define REV3_TPMS_MS583730BA01_50_HPP
#include <cstdint>

#include <core/io/I2C.hpp>
#include <core/utils/log.hpp>

namespace io = core::io;
namespace log = core::log;

namespace TPMS::dev {
    /**
     *Class for the MS5837-30BA I2C Pressure and Temperature Sensor
     */
    class MS5837 {
    public:
        // Constructor
        MS5837(io::I2C& i2c_bus);

        bool init();

        /**
         * Get the pressure from the MS5837
         * @return pressure in mbar
         */
        uint32_t readPressure();

        /**
         * Gets the temperature from the MS5837
         * @return temperature in degrees Celsius
         */
        uint32_t readTemperature();
        uint16_t calibrationData[7]{};

    private:
        // 7-bit address for the I2C

        static constexpr uint8_t I2C_ADDRESS = 0x76 << 1;
        io::I2C& i2c;

    };
}


#endif // REV3_TPMS_MS583730BA01_50_HPP
