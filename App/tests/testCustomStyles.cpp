/** @file           testCustomStyles.cpp
 *  @author         @RyanMcB8
 *  @brief          A file which stores the necessary test scripts
 *                  for all the testable functions within the `CustomStyles`
 *                  files.
 */

/*  Adding in the necessary headers and libraries. */
#include "CustomStyles.hpp"
#include "unitTestMacros.hpp"

/*  Testing the VerticalSliderLookAndFeel class. */
bool testVerticalSliderLookAndFeel(void){
    bool passFail = true;
    VerticalSliderLookAndFeel LandF;

    /*  Dial colour */
    SET_GET_COLOUR(LandF, setDialColour, getDialColour, VerticalSliderLookAndFeel, passFail);

    /*  Dial edge colour */
    SET_GET_COLOUR(LandF, setDialEdgeColour, getDialEdgeColour, VerticalSliderLookAndFeel, passFail);

    /*  Tick colour */
    SET_GET_COLOUR(LandF, setTickColour, getTickColour, VerticalSliderLookAndFeel, passFail);

    /*  Track colour */
    SET_GET_COLOUR(LandF, setTrackColour, getTrackColour, VerticalSliderLookAndFeel, passFail);

    /*  Background colour */
    SET_GET_COLOUR(LandF, setBackgroundColour, getBackgroundColour, VerticalSliderLookAndFeel, passFail);

    /*  Text colour */
    SET_GET_COLOUR(LandF, setTextColour, getTextColour, VerticalSliderLookAndFeel, passFail);

    /*  Track width */
    float value = 5.0;
    SET_GET_FLOAT(LandF, setTrackWidth, getTrackWidth, value, VerticalSliderLookAndFeel, passFail);

    /*  Tick thickness */
    value = 2.0f;
    SET_GET_FLOAT(LandF, setTickThickness, getTickThickness, value, VerticalSliderLookAndFeel, passFail);

    /*  Tick width */
    value = 4.0f;
    SET_GET_FLOAT(LandF, setTickThickness, getTickThickness, value, VerticalSliderLookAndFeel, passFail);

#ifdef USE_CIRCLE_FINGER 
    /*  Finger radius */
    value = 8.0f;
    SET_GET_FLOAT(LandF, setFingerRadius, getFingerRadius, value, VerticalSliderLookAndFeel, passFail);

#else
    /*  Finger width */
    value = 3.0f;
    SET_GET_FLOAT(LandF, setFingerWidth, getFingerWidth, value, VerticalSliderLookAndFeel, passFail);

    /*  Finger height */
    value = 8.0f;
    SET_GET_FLOAT(LandF, setFingerHeight, getFingerHeight, value, VerticalSliderLookAndFeel, passFail);

#endif
    return passFail;
}

/** @brief  Testing the CalibrationSliderLookAndFeel class's methods.  */
bool testCalibrationSliderLookAndFeel(void){
    bool passFail = true;
    CalibrationSliderLookAndFeel LandF;

    /*  Empty track colour */
    SET_GET_COLOUR(LandF, setEmptyTrackColour, getEmptyTrackColour, CalibrationSliderLookAndFeel, passFail);

    /*  Full track colour */
    SET_GET_COLOUR(LandF, setFullTrackColour, getFullTrackColour, CalibrationSliderLookAndFeel, passFail);
    
    /*  First thumb colour */
    SET_GET_COLOUR(LandF, setMinThumbColour, getMinThumbColour, CalibrationSliderLookAndFeel, passFail);
    
    /*  Second thumb colour */
    SET_GET_COLOUR(LandF, setMaxThumbColour, getMaxThumbColour, CalibrationSliderLookAndFeel, passFail);

    /*  Track width */
    float value = 5.0;
    SET_GET_FLOAT(LandF, setTrackWidth, getTrackWidth, value, CalibrationSliderLookAndFeel, passFail);

    return passFail;
}

/** @brief  Testing the NeedleLookAndFeel class's methods.  */
bool testNeedleLookAndFeel(void){
    bool passFail = true;
    NeedleLookAndFeel LandF;

    /*  Empty track colour */
    SET_GET_COLOUR(LandF, setNeedleColour, getNeedleColour, NeedleLookAndFeel, passFail);

    return passFail;
}

int main(){
    bool success = true;
    success &= testVerticalSliderLookAndFeel();
    success &= testCalibrationSliderLookAndFeel();
    success &= testNeedleLookAndFeel();

    /*  As success = true means the test has been passed,
        the sucess value needs to be inverted for the return
        result as return 0 is pass in the normal way. */
    return !success;
}