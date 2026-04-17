#pragma once

#include <functional>
#include "ads1115rpi.h"  // From ADS1115 submodule

/**
 * @brief Wrapper around ADS1115rpi library (https://github.com/berndporr/rpi_ads1115) by Bernd Porr that provides an interface for pressure sensing.
 */
class PressureSensor {
public:
    using PressureCallback = std::function<void(float)>;

    PressureSensor() = default;
    ~PressureSensor() { stop(); }

    PressureSensor(const PressureSensor&) = delete;
    PressureSensor& operator=(const PressureSensor&) = delete;

    
    void registerCallback(PressureCallback cb) {
        callback = cb;
        adc.registerCallback([this](float voltage) {
            if (callback) {
                callback(voltage);
            }
        });
    }

 
    void start(ADS1115settings settings = getDefaultSettings()) {
        adc.start(settings);
    }


    void stop() {
        adc.stop();
    }
  
    static ADS1115settings getDefaultSettings() {
        ADS1115settings s;
        s.samplingRate = ADS1115settings::FS128HZ;
        s.channel = ADS1115settings::AIN0;
        s.pgaGain = ADS1115settings::FSR0_512; 
        return s;
    }

    ADS1115rpi& getADC() { return adc; }

private:
    ADS1115rpi adc;
    PressureCallback callback;
};