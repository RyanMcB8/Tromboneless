// An exponential filter for smoothing timeseries signal in realtime.
// Assumes signal values are integer, you can change that.

float error = 0;
float outputVal = 0;

int expFilter(
  int newVal, // The latest measurement
  int oldVal, // The previous measurement
  int resolution, // For output, 0 is the floor, resolution is the ceiling
  float xGain){ // Input value multiplied by this factor before passing through filter
  // Increasing xGain -> smaller differences unity gain point

    float diff = newVal - oldVal; // What has changed since the last sample?
    error += (diff - error) * 0.4; // For monitoring purposes

    // FILTERING
    float x = abs(diff) * xGain; // We only care about magnitude for the filter hence abs
    // The unity gain point = 1/xGain i.e. crossover point between compression and expansion behaviour
    float y = (-2/(x+1))+2; // Exponential filter where:
    // x=0 --> y=0
    // x=1 --> y=1
    // x=inf --> y=2

    // APPLY FILTER TO DIFFERENCE AND ADJUST OUTPUT
    outputVal += diff*y;

    // RESOLUTION THRESHOLDING
    if (outputVal < 0.0){
      outputVal = 0.0;
    }
    else if (outputVal > resolution - 1){
      outputVal = resolution - 1;
    }
    return (int)outputVal; // Notice conversion from float to integer,
  }
