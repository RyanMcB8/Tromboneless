#include "Layout.hpp"
#include "Widgets.hpp"
#include "Panels.hpp"

// #include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <system_error>

#include "tromboneless_data.hpp"

Layout::Layout()
{
    /* Sets the initial size of the window to be displayed to the user. */
    setSize (800, 800);

    /* Variables for the distance range. */
    minimumDistance = 5;
    maximumDistance = 60;
    stepDistance = 0.1;   
    distanceRange = 5;

    /* Variables for the pressure range. */
    minimumPressure = 0.01;
    maximumPressure = 2;
    stepPressure = 0.01;
    pressureRange = 0.5;

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    
    addAndMakeVisible (dropDownMenus);

    /* ============================== Creation of slider for the maximum and minimum ranges. ============================== */
    /* Slider */
    addAndMakeVisible (distanceSlider);
    using juce::Slider;
    distanceSlider.slider.setRange(minimumDistance, maximumDistance, stepDistance);                 /* Setting the range to be between 5 and 60cm. */
    distanceSlider.setMinDifference(distanceRange);
    distanceSlider.slider.setTextValueSuffix (" cm");      /* Adds a unit at the end of the slider so the user knows what the value means. */
    distanceSlider.slider.setMinAndMaxValues (15.0, 45.0, juce::dontSendNotification);
    distanceSlider.slider. addListener (this);  
    distanceSlider.slider.setPopupDisplayEnabled(true, true, this, 1000);
    distanceSlider.slider.setNumDecimalPlacesToDisplay(1);
    distanceSlider.slider.setLookAndFeel(&LandF);

    /* Adding labels to the slider. */    
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Slider distance");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, ((juce::String) (minimumDistance)) +" cm");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, ((juce::String) (maximumDistance))+ " cm");


    /* ============================== Creation of slider for adjusting maximum and minimum pressure amplitude. ============================== */
    /* Slider */
    addAndMakeVisible (pressureSlider);
    using juce::Slider;
    // pressureSlider.slider.setRange(minimumPressure, maximumPressure, stepPressure);
    // pressureSlider.setMinDifference(pressureRange);
    // pressureSlider.slider.addListener (this);              /* Adds a listener so that the value may be read when changed. */
    // pressureSlider.maxSlider.slider.addListener (this);              /* Adds a listener so that the value may be read when changed. */
    // pressureSlider.slider.setValue (0.2, juce::dontSendNotification);
    // pressureSlider.maxSlider.slider.setValue (1.2, juce::dontSendNotification);
    // pressureSlider.slider.setPopupDisplayEnabled(true, true, this, 1000);
    // pressureSlider.slider.setNumDecimalPlacesToDisplay(2);
    // pressureSlider.slider.setLookAndFeel(&barometerLandF);
    // pressureSlider.maxSlider.slider.setLookAndFeel(&barometerOuterLandF);

    /* Adding labels to the slider. */
    // pressureSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Pressure gain");
    // pressureSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, ((juce::String) (minimumPressure)));
    // pressureSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, ((juce::String) (maximumPressure)));

    addAndMakeVisible(calibrateEmbachure);


    /* ============================== Creation of slider for adjusting gains for each frequency band. ============================== */
    addAndMakeVisible (equalizer);
}

Layout::~Layout()
{
    return;
}


void Layout::paint (juce::Graphics& g)
{
    /* Sets the background to be the same as the users settings for other windows. */
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    /* Setting the size of the font that will be written. */
    g.setFont (juce::FontOptions (18.0f));
    
    /* Setting the colour of the text that will be written. */
    g.setColour (juce::Colours::white);
    
    /* Adds a banner at the top of the window centred with the name of the device. */
    g.drawText ("Tromboneless Parameters", getLocalBounds(), juce::Justification::centredTop, true);
    
}

//  Window resizing configuration

void Layout::resized()    
{
    auto area = getLocalBounds();
    /* Adding a margin around the edge of the window.*/
    area = area.withSizeKeepingCentre(area.getWidth()*0.94, area.getHeight()*0.94);

    /* Setting the positon of the label and combo box. */
    // auto labelBounds = buttonArea.removeFromLeft (80); //(getWidth() - 100);
    auto comboBounds = area.removeFromTop (50); //(getWidth() - 40);
    
    /* Scaling the label and combo box. */
    // comboBounds = comboBounds.withSizeKeepingCentre (200, 40);  // width, height
    dropDownMenus.setBounds (comboBounds);
    
    auto SliderBounds = area.removeFromTop(250);
    
    /* Adding the sliders to the window. */
    auto distanceSliderBounds = SliderBounds.removeFromLeft(SliderBounds.getWidth()*0.65);
    // distanceSliderBounds = distanceSliderBounds.withSizeKeepingCentre(400, 80);
    distanceSlider.setBounds (distanceSliderBounds.removeFromBottom(150));
    
    /* Pressure slider bounds */
    auto pressureSliderBounds = SliderBounds.removeFromRight(area.getWidth()*0.25);
    pressureSlider.setBounds (pressureSliderBounds);

    calibrateEmbachure.setBounds(distanceSliderBounds);

    /* Equalizer */
    /* Only allowing the synthethiser to be viewed if it is enabled. */
    if(true == synthesiserParameters.synthEnable){
        auto equalizerBounds = area.removeFromTop(350);
        equalizerBounds = equalizerBounds.withSizeKeepingCentre(area.getWidth()*0.95, equalizerBounds.getHeight());
        equalizer.setBounds(equalizerBounds);
    }
    else{
        auto equalizerBounds = area.removeFromTop(50);
        equalizerBounds = equalizerBounds.withSizeKeepingCentre(area.getWidth()*0.95, equalizerBounds.getHeight());
        equalizer.setBounds(equalizerBounds);
    }


}

bool Layout::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}

void Layout::sliderValueChanged (juce::Slider* slider) 
{
    /* Calibrated distance has changed. */
    if (slider == &distanceSlider.slider){
        trombonelessParameters.nearDistance = distanceSlider.slider.getMinValue();
        trombonelessParameters.farDistance = distanceSlider.slider.getMaxValue();
#ifdef DBG_MSG
        std::cout << "DistanceSlider min:" <<  trombonelessParameters.nearDistance << "\n";
        std::cout << "DistanceSlider max:" <<  trombonelessParameters.farDistance << "\n";
#endif
    }

    /* Calibrated pressure has changed. */
    // else if (slider == &pressureSlider.slider){
    //     trombonelessParameters.lowPressure = pressureSlider.slider.getMinValue();
    //     trombonelessParameters.highPressure = pressureSlider.slider.getMaxValue();
#ifdef DBG_MSG
        std::cout << "PressureSlider min:" <<  trombonelessParameters.lowPressure << "\n";
        std::cout << "PressureSlider max:" <<  trombonelessParameters.highPressure << "\n";
#endif
    // }

}

