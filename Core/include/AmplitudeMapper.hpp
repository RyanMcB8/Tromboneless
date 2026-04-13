#include <cstdio> 

// This code defines a class that takes a sample of breath pressure from the ADC and maps it appropriately 
// into functional values for MIDI (0-127).

class AmplitudeMapper {
    public:
        // Attributes
        float pressureSample;
        float latestExpr;
        float bottomLimit = 100;
        float upperLimit = 200;
        bool noteOn;

        // Subscriber of ADS1115 driver. 
        float getADS1115Sample(float v) {
		    return v;
        }
        void getLatestExpr(float pressureSample) {

            latestExpr = pressureSample - bottomLimit;
            latestExpr = latestExpr * (127/upperLimit);
            if (latestExpr<0){
                latestExpr = 0;
                noteOn = 0;
            }
            else if (latestExpr>127){
                latestExpr = 127;
                noteOn = 1;
            }
            else{
                //latestExpr = latestExpr
                noteOn = 1;
            }
        }

        
	};
