#include <cstdio> 

// This code defines a class that takes a sample of breath pressure from the ADC and maps it appropriately 
// into functional values for MIDI (0-127).

class AmplitudeMapper {
    public:
        // Attributes
        float pressureSample = 0.0f;
        float latestExpr = 0.0f;
        float bottomLimit = 100.0f;
        float upperLimit = 200.0f;
        bool noteOn = false;

        // Subscriber of ADS1115 driver. 
        float getADS1115Sample(float v) {
		    return v;
        }
        void getLatestExpr(float pressureSample) {

            latestExpr = pressureSample - bottomLimit;
            latestExpr = latestExpr * 127.0f/upperLimit;
            if (latestExpr<0){
                latestExpr = 0;
                noteOn = false;
            }
            else if (latestExpr>127.0f){
                latestExpr = 127.0f;
                noteOn = true;
            }
            else{
                //latestExpr = latestExpr
                noteOn = true;
            }
        }

        
	};
