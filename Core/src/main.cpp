#include <iostream>
#include <thread>
#include <chrono>

#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

/*
 * Subscriber class
 * Receives distance from sensor
 */
class ToFPrinter
{
public:
    void hasToFSample(uint16_t distance)
    {
        std::cout << "Distance: " << distance << " mm\n";
    }
};

int main()
{
    I2CBus bus("/dev/i2c-1");

    // GPIO line must match wiring of VL53L1X interrupt pin
    ToFSensor sensor(bus, 0x29, "/dev/gpiochip0", 4);

    ToFPrinter printer;

    if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }

    // Connect publisher -> subscriber via lambda
    sensor.registerCallback([&](uint16_t distance)
    {
        printer.hasToFSample(distance);
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