#include "Layout.h"

#include <fcntl.h>
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
    minimumDistance = 5;
    maximumDistance = 60;   
    distanceRange = 5;

    /* ============================== Creation of a dropdown menu that provides options for shift keying ============================== */
    
    addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.addItem ("Middle F4", SKOpt_MiddleF4);
    shiftKeyChoice.addItem ("B sharp 4", SKOpt_BSharp4);
    shiftKeyChoice.addItem ("D5", SKOpt_D5);
    shiftKeyChoice.addItem ("F5", SKOpt_F5);
    shiftKeyChoice.addItem ("A sharp 4", SKOpt_ASharp4);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    shiftKeyChoice.onChange = [this] {shiftKeyingUpdate(); };
    shiftKeyChoice.setSelectedId (0);

    /* Adding a label to be beside the shift key dropdown menu. */
    addAndMakeVisible (shiftKeySelectLabel);
    shiftKeySelectLabel.setText ("Shift keying selector:", juce::dontSendNotification);
    shiftKeySelectLabel.setJustificationType (juce::Justification::centredRight);
    shiftKeySelectLabel.attachToComponent(&shiftKeyChoice, true);

    /* ============================== Creation of slider for the maximum and minimum ranges. ============================== */
    /* Minimum. */
    addAndMakeVisible (distanceSlider);
    using juce::Slider;
    distanceSlider.setSliderStyle(Slider::TwoValueHorizontal);
    distanceSlider.setRange(minimumDistance, maximumDistance, 0.1);                 /* Setting the range to be between 5 and 60cm. */
    distanceSlider.hideTextBox(false);
    distanceSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    distanceSlider.setTextValueSuffix (" cm");      /* Adds a unit at the end of the slider so the user knows what the value means. */
    distanceSlider.addListener (this);              /* Adds a listener so that the value may be read when changed. */
    distanceSlider.setMinAndMaxValues (15.0, 45.0, juce::dontSendNotification);
    distanceSlider.setPopupDisplayEnabled(true, true, this, 1000);
    distanceSlider.setNumDecimalPlacesToDisplay(1);

    addAndMakeVisible (distanceLabel);
    distanceLabel.setText ("Slider distance range.", juce::dontSendNotification); /* Adding text to the label. */
    distanceLabel.attachToComponent (&distanceSlider, false);        /* Attaching the label to the slider*/

    addAndMakeVisible (distanceMinLabel);
    distanceMinLabel.setText (juce::String(minimumDistance), juce::dontSendNotification); /* Adding text to the label. */
    distanceMinLabel.attachToComponent (&distanceSlider, true);        /* Attaching the label to the slider*/

    addAndMakeVisible (distanceMaxLabel);
    distanceMaxLabel.setText (juce::String(maximumDistance), juce::dontSendNotification); /* Adding text to the label. */

    // Value & 	getMinValueObject () can return the lower thumbs value (minimum distance)


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
    auto labelBounds = buttonArea.removeFromLeft (80); //(getWidth() - 100);
    auto comboBounds = buttonArea.removeFromLeft (400); //(getWidth() - 40);

    /* Scaling the label and combo box. */
    comboBounds = comboBounds.withSizeKeepingCentre (200, 40);  // width, height
    labelBounds = labelBounds.withSizeKeepingCentre (200, 40);
    shiftKeySelectLabel.setBounds (labelBounds);
    shiftKeyChoice.setBounds (comboBounds);

    /* Adding the sliders to the window. */
    auto sliderBounds = comboBounds.removeFromTop(200);
    sliderBounds = sliderBounds.removeFromLeft(50);
    sliderBounds = sliderBounds.withSizeKeepingCentre(200, 40);
    distanceSlider.setBounds (30, 100, getWidth() - 60, 20);

    distanceMaxLabel.setBounds(getWidth()-30, 100, 40, 20);

}

bool Layout::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}

