//
// Created by ppani on 4/18/2026.
//

#ifndef REV3_TPMS_MS583730BA01_50_HPP
#define REV3_TPMS_MS583730BA01_50_HPP
#include <cstdint>

#include <core/io/I2C.hpp>
#include <core/utils/log.hpp>

namespace IO = core::io;
namespace log = core::log;

namespace TPMS {
    /**
     *Class for the MS5837-30BA I2C Pressure and Temperature Sensor
     */
    class MS5837 {
    public:
        // Constructor
        MS5837(IO::I2C& i2c_bus);

        /**
         * Get the pressure from the MS5837
         * @return pressure in mbar
         */
        float readPressure();

        /**
         * Gets the temperature from the MS5837
         * @return temperature in degrees Celsius
         */
        float readTemperature();

    private:
        // 7-bit address for the I2C
        static constexpr uint8_t I2C_ADDRESS = 0x76 << 1;
    };
}


#endif // REV3_TPMS_MS583730BA01_50_HPP
