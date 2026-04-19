#pragma once
#include <cstdio> 
#include <vector>
#include <chrono>

// This code defines a class that takes a voltage level sample from
// ADS1115 representing breath pressure 
// and compares it against a baseline to determine whether it
// sufficiently high enough to switch note on.

class AmplitudeMapper {

    private:
        // Attributes
        float pressureSample = 0.0f;
        float pressureBaseline = 0.0f;
        float pressureThreshold;
        bool pressureState = false;
        std::vector<float> buffer;
        std::chrono::steady_clock::time_point startTime;

    public:
        AmplitudeMapper();
        bool calculateBaseline(float pressureSample);
        float getBaseline();
        bool noteEdge(float pressureSample);

        /** @brief              A function to get the current pressure threshold
         *                      which is set for the gate for the input blowing
         *                      pressure by the user.
         *  @retval             The floating point representation of the current
         *                      thhreshold value.
         */
        float getPressureThreshold(void);

        /** @brief              A function to set the pressure threshold which is
         *                      set for the gate of the input blowing pressure
         *                      by the user.
         *  @param  threshold   The floating point representation of the new
         *                      threshold which should be set.
         */
        void setPressureThreshold(float threshold);


	};
