/*  Adding in the necessary headers and libraries. */
#include "testCustomStyles.hpp"
#include "testPanels.hpp"
#include "testWidgets.hpp"



int main(){
    juce::ScopedJuceInitialiser_GUI guiInit;

    bool success = true;
    success &= testVerticalSliderLookAndFeel();
    success &= testCalibrationSliderLookAndFeel();
    success &= testNeedleLookAndFeel();
    success &= testPanels();
    success &= testSliders();
    success &= testSliderWithLabel();
    success &= testCalibrationSlider();
    success &= testDualRotarySlider();
    success &= testDropDownMenu();

    /*  As success = true means the test has been passed,
        the sucess value needs to be inverted for the return
        result as return 0 is pass in the normal way. */
    return !success;
}