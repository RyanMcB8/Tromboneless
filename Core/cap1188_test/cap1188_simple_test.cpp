// CAP1188 Test
#include <iostream>
#include <thread>
#include <chrono>

#include "drivers/i2c_bus.hpp"
#include "drivers/cap_sensor.hpp"

int main()
{    
    I2CBus bus("/dev/i2c-1");

    // GPIO line must match wiring of CAP1188 interrupt pin
    CAP1188 sensor(bus, 0x28, "/dev/gpiochip0", 27, 0xFF, 0x00);

    if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }

    return 0;
}