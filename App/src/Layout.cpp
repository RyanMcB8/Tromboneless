#include "Layout.h"
#include "Widgets.h"

// #include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <system_error>

#include "tromboneless_data.h"

Layout::Layout()
{
    /* Sets the initial size of the window to be displayed to the user. */
    setSize (500, 800);

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
    
    addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.addItem ("Middle F4", SKOpt_MiddleF4);
    shiftKeyChoice.addItem ("B sharp 4", SKOpt_BSharp4);
    shiftKeyChoice.addItem ("D5", SKOpt_D5);
    shiftKeyChoice.addItem ("F5", SKOpt_F5);
    shiftKeyChoice.addItem ("A sharp 4", SKOpt_ASharp4);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    shiftKeyChoice.onChange = [this] {trombonelessParameters.shiftKeyingOption = (ShiftKeyingOptions_t) shiftKeyChoice.getSelectedId(); };
    shiftKeyChoice.setSelectedId (0);

    /* Adding a label to be beside the shift key dropdown menu. */
    addAndMakeVisible (shiftKeySelectLabel);
    shiftKeySelectLabel.setText ("Transposition selector:", juce::dontSendNotification);
    shiftKeySelectLabel.setJustificationType (juce::Justification::centredRight);
    shiftKeySelectLabel.attachToComponent(&shiftKeyChoice, true);

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
    
    /* Adding labels to the slider. */    
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Slider distance");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, ((juce::String) (minimumDistance)) +" cm");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, ((juce::String) (maximumDistance))+ " cm");


    /* ============================== Creation of slider for adjusting maximum and minimum pressure amplitude. ============================== */
    /* Slider */
    addAndMakeVisible (pressureSlider);
    using juce::Slider;
    pressureSlider.slider.setRange(minimumPressure, maximumPressure, stepPressure);
    pressureSlider.setMinDifference(pressureRange);
    pressureSlider.slider.addListener (this);              /* Adds a listener so that the value may be read when changed. */
    pressureSlider.slider.setMinAndMaxValues (0.05, 1, juce::dontSendNotification);
    pressureSlider.slider.setPopupDisplayEnabled(true, true, this, 1000);
    pressureSlider.slider.setNumDecimalPlacesToDisplay(2);

    /* Adding labels to the slider. */
    pressureSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Pressure gain");
    pressureSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, ((juce::String) (minimumPressure)));
    pressureSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, ((juce::String) (maximumPressure)));

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

    auto buttonArea = area.removeFromTop (50);

    /* Setting the positon of the label and combo box. */
    // auto labelBounds = buttonArea.removeFromLeft (80); //(getWidth() - 100);
    auto comboBounds = area.removeFromTop (10); //(getWidth() - 40);

    /* Scaling the label and combo box. */
    comboBounds = comboBounds.withSizeKeepingCentre (200, 40);  // width, height
    shiftKeyChoice.setBounds (comboBounds);

    /* Adding the sliders to the window. */
    auto distanceSliderBounds = area.removeFromTop(150);
    distanceSliderBounds = distanceSliderBounds.withSizeKeepingCentre(400, 80);
    distanceSlider.setBounds (distanceSliderBounds);

    /* Pressure slider bounds */
    auto pressureSliderBounds = area.removeFromTop(150);
    pressureSliderBounds = pressureSliderBounds.withSizeKeepingCentre(400, 80);
    pressureSlider.setBounds (pressureSliderBounds);

    /* Equalizer */
    auto equalizerBounds = area.removeFromTop(250);
    equalizerBounds = equalizerBounds.withSizeKeepingCentre(area.getWidth()-20, 220);
    equalizer.setBounds(equalizerBounds);


}

bool Layout::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}

