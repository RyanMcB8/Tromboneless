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
    shiftKeySelectLabel.setText ("Shift keying selector:", juce::dontSendNotification);
    shiftKeySelectLabel.setJustificationType (juce::Justification::centredRight);
    shiftKeySelectLabel.attachToComponent(&shiftKeyChoice, true);

    /* ============================== Creation of slider for the maximum and minimum ranges. ============================== */
    /* Slider */
    addAndMakeVisible (distanceSlider);
    using juce::Slider;
    distanceSlider.setRange(minimumDistance, maximumDistance, stepDistance);                 /* Setting the range to be between 5 and 60cm. */
    distanceSlider.setMinDifference(distanceRange);
    // distanceSlider.hideTextBox(false);
    distanceSlider.setTextValueSuffix (" cm");      /* Adds a unit at the end of the slider so the user knows what the value means. */
    distanceSlider.setMinAndMaxValues (15.0, 45.0, juce::dontSendNotification);
    distanceSlider. addListener (this);  
    distanceSlider.setPopupDisplayEnabled(true, true, this, 1000);
    distanceSlider.setNumDecimalPlacesToDisplay(1);
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::UpperCentre, "Slider Title");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerLeft, "Min");
    distanceSlider.CreateLabel(SliderWithLabel::LabelPositions_t::LowerRight, "Max");

    /* main label */
    addAndMakeVisible (distanceLabel);
    distanceLabel.setText ("Slider distance range.", juce::dontSendNotification); /* Adding text to the label. */
    distanceLabel.attachToComponent (&distanceSlider, false);        /* Attaching the label to the slider*/
    distanceLabel.setMinimumHorizontalScale(2);

    /* Minimum value label */
    addAndMakeVisible (distanceMinLabel);
    distanceMinLabel.setText (juce::String(minimumDistance) + " cm", juce::dontSendNotification); /* Adding text to the label. */
    distanceMinLabel.attachToComponent (&distanceSlider, true);        /* Attaching the label to the slider*/

    /* Maximum value label */
    addAndMakeVisible (distanceMaxLabel);
    distanceMaxLabel.setText (juce::String(maximumDistance) + " cm", juce::dontSendNotification); /* Adding text to the label. */

    /* ============================== Creation of slider for adjusting maximum and minimum pressure amplitude. ============================== */
    /* Slider */
    addAndMakeVisible (pressureSlider);
    using juce::Slider;
    pressureSlider.setRange(minimumPressure, maximumPressure, stepPressure);
    pressureSlider.setMinDifference(pressureRange);
    pressureSlider.hideTextBox(false);
    pressureSlider.addListener (this);              /* Adds a listener so that the value may be read when changed. */
    pressureSlider.setMinAndMaxValues (0.05, 1, juce::dontSendNotification);
    pressureSlider.setPopupDisplayEnabled(true, true, this, 1000);
    pressureSlider.setNumDecimalPlacesToDisplay(2);

    /* main label */
    addAndMakeVisible (pressureLabel);
    pressureLabel.setText ("Setting the maximum and minimum relative pressures.", juce::dontSendNotification); /* Adding text to the label. */
    pressureLabel.attachToComponent (&pressureSlider, false);        /* Attaching the label to the slider*/
    pressureLabel.setMinimumHorizontalScale(2);

    /* Minimum value label */
    addAndMakeVisible (pressureMinLabel);
    pressureMinLabel.setText (juce::String(minimumPressure), juce::dontSendNotification); /* Adding text to the label. */
    pressureMinLabel.attachToComponent (&pressureSlider, true);        /* Attaching the label to the slider*/

    /* Maximum value label */
    addAndMakeVisible (pressureMaxLabel);
    pressureMaxLabel.setText (juce::String(maximumPressure), juce::dontSendNotification); /* Adding text to the label. */

    /* ============================== Creation of slider for adjusting gains for each frequency band. ============================== */
    addAndMakeVisible (lowFreqSlider);
    pressureSlider.setRange(0, 5, 0.1);
    pressureSlider.addListener (this); 
    pressureSlider.setPopupDisplayEnabled(true, true, this, 1000);
    pressureSlider.setNumDecimalPlacesToDisplay(2);




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
    g.drawText ("Tromboneless.tech", getLocalBounds(), juce::Justification::centredTop, true);

}

//  Window resizing configuration

void Layout::resized()    
{
    auto area = getLocalBounds();

    auto buttonArea = area.removeFromTop (100);

    /* Setting the positon of the label and combo box. */
    // auto labelBounds = buttonArea.removeFromLeft (80); //(getWidth() - 100);
    auto comboBounds = buttonArea.removeFromLeft (380); //(getWidth() - 40);

    /* Scaling the label and combo box. */
    comboBounds = comboBounds.withSizeKeepingCentre (200, 40);  // width, height
    shiftKeyChoice.setBounds (comboBounds);

    /* Adding the sliders to the window. */
    auto sliderBounds = comboBounds.removeFromTop(200);
    sliderBounds = sliderBounds.removeFromLeft(50);
    sliderBounds = sliderBounds.withSizeKeepingCentre(200, 40);
    distanceSlider.setBounds (50, 120, getWidth() - 105, 20);
    distanceMaxLabel.setBounds(getWidth()-55, 120, 45, 20);

    /* Pressure slider bounds */
    pressureSlider.setBounds (50, 220, getWidth() - 105, 20);
    pressureMaxLabel.setBounds(getWidth()-55, 220, 45, 20);

    /* Frequency */
    lowFreqSlider.setBounds(100, 400, 20, 100);

}

bool Layout::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}

