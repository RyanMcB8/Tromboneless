#include "drivers/i2c_bus.hpp"
#include "vl53l1_platform.h"
#include "VL53L1X_api.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

int main()
{
    I2CBus bus("/dev/i2c-1");

    if (!bus.openBus())
    {
        std::cerr << "Failed to open I2C bus\n";
        return 1;
    }

    VL53L1_PlatformInit(bus);

    uint8_t state = 0;
    int attempts = 100;

    while (!state && attempts-- > 0)
    {
        VL53L1X_ERROR rc = VL53L1X_BootState(0x52, &state);

        std::cout << "BootState rc=" << static_cast<int>(rc)
                  << " state=" << static_cast<int>(state) << '\n';

        if (rc != 0)
        {
            std::cerr << "BootState failed\n";
            return 1;
        }

        if (!state)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    if (!state)
    {
        std::cerr << "Sensor never reported booted\n";
        return 1;
    }

    uint16_t id = 0;
    VL53L1X_ERROR rc = VL53L1X_GetSensorId(0x52, &id);

    std::cout << "GetSensorId rc=" << static_cast<int>(rc)
              << " id=0x" << std::hex << id << std::dec << '\n';

    if (rc != 0)
    {
        std::cerr << "GetSensorId failed\n";
        return 1;
    }

    if (id != 0xEACC)
    {
        std::cerr << "Unexpected sensor ID\n";
        return 1;
    }

    std::cout << "Basic ULD platform test passed\n";
    return 0;
}