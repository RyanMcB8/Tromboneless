#include <iostream>
#include <iomanip>

#include "drivers/tof_sensor.hpp"

int main()
{
    ToFSensor sensor;

    if (!sensor.initialise())
    {
        std::cerr << "Failed to initialise ToF sensor\n";
        return 1;
    }

    uint8_t value = 0;

    // Currently hardcoded to read the sensor's model ID register (0x010F)
    if (!sensor.readRegisterByte(0x010F, value))
    {
        std::cerr << "Failed to read register\n";
        return 1;
    }

    std::cout << "Register value: 0x"
              << std::hex << std::uppercase
              << static_cast<int>(value)
              << '\n';

    return 0;
}