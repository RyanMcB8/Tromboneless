#include <cstdint>
#include <iostream>
#include "main.hpp"
#include "tromboneSynth.hpp"
#include "tof_sensor.hpp"

class GetDistance {
public:
    int hasTOFsample(uint16_t distance) {
        if (distance > 500) distance = 500;
        int scaled_bend = 8192 - (distance * 8192) / 500;
        std::cout << scaled_bend << "\n";
        return scaled_bend;
    }
};

int main()
{
    I2CBus bus("/dev/i2c-1");

    // GPIO line must match wiring of VL53L1X interrupt pin
    ToFSensor sensor(bus, 0x29, "/dev/gpiochip0", 4);

    // ToFPrinter printer;

    if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }

    // Connect publisher -> subscriber via lambda
    sensor.registerCallback([&](uint16_t distance)
    {
        // printer.hasToFSample(distance);
    });

    // Start blocking GPIO + sensor
    sensor.start();

    // Main thread idle (system is event-driven now)
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
