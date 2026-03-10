#include "profiler.h"

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <system_error>

Profiler::Profiler()
{
    /* Sets the initial size of the window to be displayed to the user. */
    setSize (500, 800);

    // Creation of a dropdown menu that provides options for shift keying
    addAndMakeVisible (module_label);
    module_label.setText ("Shift keying selector:", juce::dontSendNotification);
    module_label.setJustificationType (juce::Justification::centredRight);
    
    addAndMakeVisible (moduleComboBox);
    moduleComboBox.addItem ("Middle F4", SKOpt_MiddleF4);
    moduleComboBox.addItem ("B sharp 4", SKOpt_BSharp4);
    moduleComboBox.addItem ("D5", SKOpt_D5);
    moduleComboBox.addItem ("F5", SKOpt_F5);
    moduleComboBox.addItem ("A sharp 4", SKOpt_ASharp4);
    moduleComboBox.setSelectedId (0);

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

Profiler::~Profiler()
{
    return;
}


void Profiler::paint (juce::Graphics& g)
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

void Profiler::resized()    
{
    auto area = getLocalBounds();

    auto buttonArea = area.removeFromTop (60);

    auto labelBounds = buttonArea.removeFromLeft (60);
    auto comboBounds = buttonArea.removeFromLeft (400);
    comboBounds = comboBounds.withSizeKeepingCentre (200, 20);  // width, height
    labelBounds = labelBounds.withSizeKeepingCentre (200, 20);
    module_label.setBounds (labelBounds);
    moduleComboBox.setBounds (comboBounds);

}

bool Profiler::keyPressed(const juce::KeyPress& key)
{
    if (key == juce::KeyPress::spaceKey)
    {
        return true;  // Key was handled
    }
    return false;  // Key was not handled
}