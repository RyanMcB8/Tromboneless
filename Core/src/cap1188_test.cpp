// CAP1188 Test
#include <iostream>
#include <thread>
#include <chrono>

#include "drivers/i2c_bus.hpp"
#include "drivers/cap_sensor.hpp"

class CAP1188Printer{
    public:
        void hasCAPSample(uint8_t delta){
            std::cout << "Delta Count: " << delta;
        }
};

int main()
{    
    I2CBus bus("/dev/i2c-1");

    // GPIO line must match wiring of CAP1188 interrupt pin
    CAP1188 sensor(bus, 0x28, "/dev/gpiochip0", 27, 1, 1);

    CAP1188Printer printer;

    if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }

    // Connect publisher → subscriber via lambda
    sensor.registerCallback([&](uint16_t delta)
    {
        printer.hasCAPSample(delta);
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

