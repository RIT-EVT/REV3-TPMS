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
    // UART2 -> TX PA2, RX, PA3, UART1 TX PA9, RX PA10
    io::UART& uart = io::getUART<io::Pin::PA_9, io::Pin::PA_10>(9600);

    // Setup I2C
    io::I2C& i2c = io::getI2C<io::Pin::PB_6, io::Pin::PB_7>();

    // Setup MS5837
    TPMS::dev::MS5837 pressureSensor = TPMS::dev::MS5837(i2c);

    while (1) {
        uint32_t pressure = pressureSensor.readPressure();
        // Read user input
        for (int i = 0; i < 7; i++) {
            uart.printf("C%d Cal: %d\r\n", i, pressureSensor.calibrationData[i]);
        }
        //uart.printf("Pressure(mbar): %d.%d\r\n", (int) (pressure /10), (int) (pressure % 10));
        // wait 500 ms
        time::wait(500);
    }
}
