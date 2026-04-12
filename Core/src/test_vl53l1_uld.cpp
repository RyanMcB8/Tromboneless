#include "drivers/i2c_bus.hpp"
#include "vl53l1_platform.h"

extern "C" {
#include "VL53L1X_api.h"
}

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

    std::cout << "I2C bus opened\n";

    VL53L1_PlatformInit(bus);
    std::cout << "Platform layer initialised\n";

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
        std::cerr << "Sensor did not report booted state\n";
        return 1;
    }

    uint16_t id = 0;
    VL53L1X_ERROR rc = VL53L1X_GetSensorId(0x52, &id);

    std::cout << "GetSensorId rc=" << static_cast<int>(rc)
              << " id=0x" << std::hex << id << std::dec << '\n';

    if (rc != 0 || id != 0xEACC)
    {
        std::cerr << "Sensor ID check failed\n";
        return 1;
    }

    rc = VL53L1X_SensorInit(0x52);
    std::cout << "SensorInit rc=" << static_cast<int>(rc) << '\n';

    if (rc != 0)
    {
        std::cerr << "SensorInit failed\n";
        return 1;
    }

    // Start with a conservative, defined profile
    rc = VL53L1X_SetDistanceMode(0x52, 1); // 1 = short, 2 = long
    std::cout << "SetDistanceMode rc=" << static_cast<int>(rc) << '\n';
    if (rc != 0) return 1;

    rc = VL53L1X_SetTimingBudgetInMs(0x52, 15);
    std::cout << "SetTimingBudget rc=" << static_cast<int>(rc) << '\n';
    if (rc != 0) return 1;

    rc = VL53L1X_SetInterMeasurementInMs(0x52, 15);
    std::cout << "SetInterMeasurement rc=" << static_cast<int>(rc) << '\n';
    if (rc != 0) return 1;

    rc = VL53L1X_StartRanging(0x52);
    std::cout << "StartRanging rc=" << static_cast<int>(rc) << '\n';
    if (rc != 0) return 1;

    for (int sample = 0; sample < 10; ++sample)
    {
        uint8_t ready = 0;
        int waitLoops = 200;

        while (!ready && waitLoops-- > 0)
        {
            rc = VL53L1X_CheckForDataReady(0x52, &ready);
            if (rc != 0)
            {
                std::cerr << "CheckForDataReady failed\n";
                return 1;
            }

            if (!ready)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        }

        if (!ready)
        {
            std::cerr << "Timed out waiting for data ready\n";
            return 1;
        }

        uint16_t distance = 0;
        uint8_t status = 255;

        rc = VL53L1X_GetRangeStatus(0x52, &status);
        if (rc != 0)
        {
            std::cerr << "GetRangeStatus failed\n";
            return 1;
        }

        rc = VL53L1X_GetDistance(0x52, &distance);
        if (rc != 0)
        {
            std::cerr << "GetDistance failed\n";
            return 1;
        }

        rc = VL53L1X_ClearInterrupt(0x52);
        if (rc != 0)
        {
            std::cerr << "ClearInterrupt failed\n";
            return 1;
        }

        std::cout << "Sample " << sample
                  << " distance=" << distance
                  << " mm status=" << static_cast<int>(status)
                  << '\n';
    }

    rc = VL53L1X_StopRanging(0x52);
    std::cout << "StopRanging rc=" << static_cast<int>(rc) << '\n';

    std::cout << "ULD ranging test passed\n";
    return 0;
}