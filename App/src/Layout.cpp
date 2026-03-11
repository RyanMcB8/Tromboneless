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

    // Creation of a dropdown menu that provides options for shift keying
    
    addAndMakeVisible (shiftKeyChoice);
    shiftKeyChoice.addItem ("Middle F4", SKOpt_MiddleF4);
    shiftKeyChoice.addItem ("B sharp 4", SKOpt_BSharp4);
    shiftKeyChoice.addItem ("D5", SKOpt_D5);
    shiftKeyChoice.addItem ("F5", SKOpt_F5);
    shiftKeyChoice.addItem ("A sharp 4", SKOpt_ASharp4);
    
    /* This line is the one responsible for calling the shiftKeyingUpdate function when the choice changes. */
    shiftKeyChoice.onChange = [this] {shiftKeyingUpdate(); };
    shiftKeyChoice.setSelectedId (0);

    

    addAndMakeVisible (shiftKeySelectLabel);
    shiftKeySelectLabel.setText ("Shift keying selector:", juce::dontSendNotification);
    shiftKeySelectLabel.setJustificationType (juce::Justification::centredRight);
    shiftKeySelectLabel.attachToComponent(&shiftKeyChoice, true);

    juce::File imageFile = juce::File::getCurrentWorkingDirectory().getChildFile("resources/circle_gold.png");
    if (imageFile.existsAsFile())
    {
        auto buttonImage = juce::ImageCache::getFromFile (imageFile);
        if (buttonImage.isValid())
        {
            // This is where the images will be set if they are added in the future.
            // imageButton.setImages()
        }
    }

    // imageButton.onClick = ;

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

}

bool Layout::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}

