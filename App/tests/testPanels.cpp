/** @file           testPanels.cpp
 *  @author         @RyanMcB8
 *  @brief          A file which stores the necessary test scripts
 *                  for all the testable functions within the `Panels`
 *                  files.
 */

/*  Adding in the necessary headers and libraries. */
#include "testPanels.hpp"

/*  Testing the panels class. */
bool testPanels(void){
    bool passFail = true;
    Panels testClass;
    SET_GET_COLOUR(testClass, setBackgroundColour, getBackgroundColour, Panels, passFail);

    return passFail;
}

/*  Testing the Sliders class. */
bool testSliders(void){
    bool passFail = true;
    CoreWrapper* coreWrapper = new CoreWrapper(true); 
    Sliders testClass(*coreWrapper);

    /* Minimum distance. */
    float value = 1.0;
    SET_GET_FLOAT(testClass, setMinimumDistance, getMinimumDistance, value, Sliders, passFail);

    /*  Maximum distance. */
    value = 10.0;
    SET_GET_FLOAT(testClass, setMaximumDistance, getMaximumDistance, value, Sliders, passFail);

    /*  Minimum distance movable. */
    value = 0.5;
    SET_GET_FLOAT(testClass, setStepDistance, getStepDistance, value, Sliders, passFail);

    /*  Minimum difference between fingers. */
    value = 2.0;
    SET_GET_FLOAT(testClass, setDistanceRange, getDistanceRange, value, Sliders, passFail);

    return passFail;
}
