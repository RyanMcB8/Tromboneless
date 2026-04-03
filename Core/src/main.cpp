#include <cstdint>
#include <iostream>

#include "tromboneSynth.hpp"

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
    return 0;
}
