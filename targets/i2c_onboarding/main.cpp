/**
 * Basic main to get pressure reading from the MS583730BA01
 */

#include "dev/MS583730BA01_50.hpp"

#include <core/io/UART.hpp>
#include <core/io/pin.hpp>
#include <core/manager.hpp>
#include <core/utils/time.hpp>

namespace io = core::io;
namespace time = core::time;

int main() {
    // Initialize system
    core::platform::init();

    // Setup UART
    io::UART& uart = io::getUART<io::Pin::PA_2, io::Pin::PA_3>(9600);

    // Setup I2C
    io::I2C& i2c = io::getI2C<io::Pin::PB_6, io::Pin::PB_7>();

    // Setup MS5837
    TPMS::dev::MS5837 pressureSensor = TPMS::dev::MS5837(i2c);

    uart.printf("hi\r\n");

    while (1) {
        float pressure = pressureSensor.readPressure();
        // Read user input
        uart.printf("Pressure(mbar): %.2f\r\n", pressure);
        // wait 500 ms
        time::wait(500);
    }
}
