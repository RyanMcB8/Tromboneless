// CAP1188 Test
#include <iostream>
#include <thread>
#include <chrono>

#include "drivers/i2c_bus.hpp"
#include "drivers/cap_sensor.hpp"


class CAP1188Printer {
public:
    void hasCAPSample(int8_t delta) {
        using namespace std::chrono;

        auto now = steady_clock::now();

        if (first) {
            last = now;
            first = false;
        }

        duration<float, std::milli> dt = now - last;
        last = now;

        int strength;
        if (delta >= 110)
            strength = 4;
        else if (delta >= 90)
            strength = 3;
        else if (delta >= 70)
            strength = 2;
        else if (delta >= 50)
            strength = 1;
        else
            strength = 0;

        std::cout
            << "C1 Delta: " << static_cast<int>(delta)
            << "\tStrength: " << strength
            << "\tΔt: " << dt.count() << " ms"
            << std::endl;
    }

private:
    std::chrono::steady_clock::time_point last;
    bool first = true;
};

int main()
{    
    I2CBus bus("/dev/i2c-1");

    // GPIO line must match wiring of CAP1188 interrupt pin
    CAP1188 sensor(bus, 0x28, "/dev/gpiochip0", 27, 22, 0x01, 0x01);

    CAP1188Printer printer;

    if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }


    sensor.recalibratePins(0x01);

    // Connect publisher → subscriber via lambda
    sensor.registerCallback([&](int8_t delta)
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

