#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

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
    // Create shared Linux I2C bus object
    I2CBus bus("/dev/i2c-1");

    // Create sensor object
    ToFSensor sensor(bus, 0x29, "/dev/gpiochip0", 4);

    ToFPrinter printer;

  // Connect publisher -> subscriber via lambda
    sensor.registerCallback([&](uint16_t distance)
    {
        printer.hasToFSample(distance);
    });

    // Initialise sensor
    if (!sensor.initialise())
    {
        std::cerr << "Sensor initialisation failed" << std::endl;
        return 1;
    }

    std::cout << "Sensor initialised successfully" << std::endl;

    // Start ranging + worker thread
    sensor.start();

    std::cout << "Sensor started, waiting for data..." << std::endl;

    // Keep main alive so the worker thread can continue running
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}