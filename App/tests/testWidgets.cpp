/** @file           testWidgets.cpp
 *  @author         @RyanMcB8
 *  @brief          A file which stores the necessary test scripts
 *                  for all the testable functions within the `Widgets`
 *                  files.
 */

/*  Adding in the necessary headers and libraries. */
#include "testWidgets.hpp"

bool testSliderWithLabel(void){
    bool passFail = true;
    SliderWithLabel testClass = SliderWithLabel(juce::Slider::SliderStyle::LinearHorizontal);

    /* Testing create label. */
    if (testClass.CreateLabel(SliderWithLabel::LabelPositions_t::LowerCentre, (juce::String) "TEST") != SliderWithLabel::InitialiseLabelErrors_t::LabelOK){
        std::cerr << "[FAIL]    CreateLabel error in the SliderWithLabelClass \n";    
        passFail &= false;
    }

    /* Testing Label Bounds. */
    int value = 10;
    SET_GET_INT(testClass, setTopLabelBounds, getTopLabelBounds, value, SliderWithLabel, passFail);
    SET_GET_INT(testClass, setLeftLabelBounds, getLeftLabelBounds, value, SliderWithLabel, passFail);
    SET_GET_INT(testClass, setRightLabelBounds, getRightLabelBounds, value, SliderWithLabel, passFail);
    SET_GET_INT(testClass, setBottomLabelBounds, getBottomLabelBounds, value, SliderWithLabel, passFail);

    return passFail;
}


bool testCalibrationSlider(void){
    bool passFail = true;
    CalibrationSlider testClass;
    float value = (testClass.slider.getMaxValue() - testClass.slider.getMinValue())/2.0f;

    SET_GET_FLOAT(testClass, setMinDifference, getMinDifference, value, CalibrationSlider, passFail);
    return passFail;
}


bool testDualRotarySlider(void){
    bool passFail = true;
    DualRotarySlider testClass;

    /*  Testing the minimum slider radius. */
    float value = 0.1;
    SET_GET_FLOAT(testClass, setMinSliderRadius, getMinSliderRadius, value, DualRotarySlider, passFail);

    /*  Testing the maximum slider radius. */
    value = 0.95;
    SET_GET_FLOAT(testClass, setMaxSliderRadius, getMaxSliderRadius, value, DualRotarySlider, passFail);

    /*  Testing label height. */
    value = 20;
    SET_GET_FLOAT(testClass, setLabelHeight, getLabelHeight, value, DualRotarySlider, passFail);

    /*  Testing the minimum limit. */
    value = 0.3;
    SET_GET_FLOAT(testClass, setMinLimit, getMinLimit, value, DualRotarySlider, passFail);

    /*  Testing the maximum limit. */
    value = 20.0;
    SET_GET_FLOAT(testClass, setMaxLimit, getMaxLimit, value, DualRotarySlider, passFail);

    /*  Testing minimum difference. */
    value = (testClass.getMaxLimit() - testClass.getMinLimit())/3.0f;
    SET_GET_FLOAT(testClass, setMinDifference, getMinDifference, value, DualRotarySlider, passFail);

    /*  Testing the interval. */
    value = 1.0;
    SET_GET_FLOAT(testClass, setInterval, getInterval, value, DualRotarySlider, passFail);

    /*  Testing the minimum angle. */
    value = (1.2 * M_PI);
    SET_GET_FLOAT(testClass, setMinAngle, getMinAngle, value, DualRotarySlider, passFail);

    /*  Testing the maximum angle. */
    value = (2.75*M_PI);
    SET_GET_FLOAT(testClass, setMaxAngle, getMaxAngle, value, DualRotarySlider, passFail);

    return passFail;
}


bool testBarometer(void){
    bool passFail = true;
    CoreWrapper coreWrapper(true); 
    Barometer testClass(coreWrapper);

    /*  Background colour. */
    SET_GET_COLOUR(testClass, setBackgroundColour, getBackgroundColour, Barometer, passFail);
    /*  Text colour. */
    SET_GET_COLOUR(testClass, setTextColour, getTextColour, Barometer, passFail);
    /*  Boarder colour. */
    SET_GET_COLOUR(testClass, setBoarderColour, getBoarderColour, Barometer, passFail);
    /*  Edge colour. */
    SET_GET_COLOUR(testClass, setEdgeColour, getEdgeColour, Barometer, passFail);
    
    /*  Outer radius. */
    float value = 0.95f;
    SET_GET_FLOAT(testClass, setOuterRadius, getOuterRadius, value, Barometer, passFail);
    /*  Inner radius. */
    value = 0.2f;
    SET_GET_FLOAT(testClass, setInnerRadius, getInnerRadius, value, Barometer, passFail);
    /*  Label radius. */
    value = 0.5f;
    SET_GET_FLOAT(testClass, setLabelRadius, getLabelRadius, value, Barometer, passFail);
    

    return passFail;
}


bool testDropDownMenu(void){
    bool passFail = true;
    DropDownMenu testClass;

    /*  Testing that the label text may be changed. */
    juce::String originalText = testClass.getLabelText();

    juce::String newText = (juce::String) "HelloWorld";
    if (originalText == newText) newText = (juce::String)"GoodbyeWorld";

    testClass.ChangeLabelText(newText);
    if (testClass.getLabelText() != newText){
        std::cerr << "[FAIL]    Error in either the `getLabelText` or `ChangeLabelTest` functions in the DropDownMenu class\n";
        passFail &= false;     
    }

    return passFail;
}

